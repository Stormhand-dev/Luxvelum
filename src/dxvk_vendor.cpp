#include "dxvk_vendor.h"

#include "log.h"

#include <windows.h>

#include <cctype>
#include <cstring>

namespace
{
constexpr const char* kNvidiaPciVendor = "VEN_10DE";
constexpr DWORD kMaxDxvkConfigLength = 4096;

bool ContainsIgnoringCase(const std::string& text, const char* part)
{
    const size_t length = std::strlen(part);
    if (length > text.size())
        return false;
    for (size_t start = 0; start + length <= text.size(); ++start)
    {
        size_t i = 0;
        while (i < length && std::tolower(static_cast<unsigned char>(text[start + i])) ==
                                 std::tolower(static_cast<unsigned char>(part[i])))
            ++i;
        if (i == length)
            return true;
    }
    return false;
}

bool DesktopGpuIsNvidia()
{
    DISPLAY_DEVICEA device = {};
    device.cb = sizeof(device);
    for (DWORD index = 0; EnumDisplayDevicesA(nullptr, index, &device, 0); ++index)
    {
        if ((device.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE) && PciDeviceIdIsNvidia(device.DeviceID))
            return true;
        device = {};
        device.cb = sizeof(device);
    }
    return false;
}

std::string CurrentDxvkConfig()
{
    std::string value(kMaxDxvkConfigLength, '\0');
    const DWORD length = GetEnvironmentVariableA(kDxvkConfigVariable, value.data(), kMaxDxvkConfigLength);
    value.resize(length < kMaxDxvkConfigLength ? length : 0);
    return value;
}
}

bool PciDeviceIdIsNvidia(const char* deviceId)
{
    return deviceId && ContainsIgnoringCase(deviceId, kNvidiaPciVendor);
}

bool DxvkConfigSetsVendor(const std::string& dxvkConfig)
{
    return ContainsIgnoringCase(dxvkConfig, kDxvkVendorOption);
}

std::string DxvkConfigReportingAmd(const std::string& dxvkConfig)
{
    if (dxvkConfig.empty())
        return kDxvkAmdVendorSetting;
    return dxvkConfig + "; " + kDxvkAmdVendorSetting;
}

DxvkVendorDecision DecideDxvkVendor(const DxvkVendorInputs& inputs)
{
    if (!inputs.asked)
        return {false, "", "DxvkAmdVendor=0"};
    if (!inputs.multisampling)
        return {false, "", "Multisampling=0"};
    if (inputs.systemD3D9)
        return {false, "", "d3d9.dll is the system copy, not DXVK"};
    if (!inputs.nvidiaGpu)
        return {false, "", "the desktop's graphics card is not NVIDIA, so DXVK offers RESZ already"};
    if (DxvkConfigSetsVendor(inputs.dxvkConfig))
        return {false, "", "DXVK_CONFIG already sets d3d9.customVendorId"};
    return {true, DxvkConfigReportingAmd(inputs.dxvkConfig), ""};
}

void ReportAmdToDxvkWhenAsked(bool asked, bool multisampling, bool systemD3D9)
{
    static bool decided = false;
    if (decided || !asked)
        return;
    decided = true;
    DxvkVendorInputs inputs;
    inputs.asked = asked;
    inputs.multisampling = multisampling;
    inputs.systemD3D9 = systemD3D9;
    inputs.nvidiaGpu = DesktopGpuIsNvidia();
    inputs.dxvkConfig = CurrentDxvkConfig();
    const DxvkVendorDecision decision = DecideDxvkVendor(inputs);
    if (!decision.reportAmd)
    {
        VF_LOG_INFO("DXVK vendor: left as it is (%s)", decision.reason);
        return;
    }
    if (!SetEnvironmentVariableA(kDxvkConfigVariable, decision.dxvkConfig.c_str()))
    {
        VF_LOG_ERROR("DXVK vendor: setting %s failed with Windows error %lu", kDxvkConfigVariable, GetLastError());
        return;
    }
    VF_LOG_INFO("DXVK vendor: %s=\"%s\" before DXVK starts, so it reports AMD and offers RESZ for multisampling",
                kDxvkConfigVariable, decision.dxvkConfig.c_str());
}
