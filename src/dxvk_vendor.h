#pragma once

#include <string>

constexpr const char* kDxvkConfigVariable = "DXVK_CONFIG";
constexpr const char* kDxvkVendorOption = "d3d9.customVendorId";
constexpr const char* kDxvkAmdVendorSetting = "d3d9.customVendorId = 1002";

struct DxvkVendorInputs
{
    bool asked = false;
    bool multisampling = false;
    bool systemD3D9 = false;
    bool nvidiaGpu = false;
    std::string dxvkConfig;
};

struct DxvkVendorDecision
{
    bool reportAmd = false;
    std::string dxvkConfig;
    const char* reason = "";
};

bool PciDeviceIdIsNvidia(const char* deviceId);
bool DxvkConfigSetsVendor(const std::string& dxvkConfig);
std::string DxvkConfigReportingAmd(const std::string& dxvkConfig);
DxvkVendorDecision DecideDxvkVendor(const DxvkVendorInputs& inputs);

void ReportAmdToDxvkWhenAsked(bool asked, bool multisampling, bool systemD3D9);
