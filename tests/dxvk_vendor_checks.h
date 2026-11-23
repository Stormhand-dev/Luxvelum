#pragma once

#include "dxvk_vendor.h"

namespace dxvk_vendor_checks
{
DxvkVendorInputs NvidiaBehindDxvk()
{
    DxvkVendorInputs inputs;
    inputs.asked = true;
    inputs.multisampling = true;
    inputs.systemD3D9 = false;
    inputs.nvidiaGpu = true;
    return inputs;
}

void CheckDxvkVendor()
{
    const DxvkVendorDecision bare = DecideDxvkVendor(NvidiaBehindDxvk());
    DxvkVendorInputs withOptions = NvidiaBehindDxvk();
    withOptions.dxvkConfig = "dxgi.syncInterval = 0";
    const DxvkVendorDecision appended = DecideDxvkVendor(withOptions);
    std::printf("     DXVK vendor: \"%s\"; with other options \"%s\"\n", bare.dxvkConfig.c_str(),
                appended.dxvkConfig.c_str());
    Check(bare.reportAmd && bare.dxvkConfig == "d3d9.customVendorId = 1002" && appended.reportAmd &&
              appended.dxvkConfig == "dxgi.syncInterval = 0; d3d9.customVendorId = 1002",
          "with DxvkAmdVendor=1, an NVIDIA card behind DXVK and Multisampling=1, DXVK_CONFIG asks DXVK to report AMD "
          "and keeps the options it already holds");

    DxvkVendorInputs notAsked = NvidiaBehindDxvk();
    notAsked.asked = false;
    DxvkVendorInputs noMultisampling = NvidiaBehindDxvk();
    noMultisampling.multisampling = false;
    DxvkVendorInputs systemD3D9 = NvidiaBehindDxvk();
    systemD3D9.systemD3D9 = true;
    DxvkVendorInputs amd = NvidiaBehindDxvk();
    amd.nvidiaGpu = false;
    DxvkVendorInputs ownVendor = NvidiaBehindDxvk();
    ownVendor.dxvkConfig = "D3D9.CustomVendorId = 10de";
    const DxvkVendorInputs leftAlone[] = {notAsked, noMultisampling, systemD3D9, amd, ownVendor};
    bool anyReported = false;
    for (const DxvkVendorInputs& inputs : leftAlone)
    {
        const DxvkVendorDecision decision = DecideDxvkVendor(inputs);
        anyReported = anyReported || decision.reportAmd || !decision.dxvkConfig.empty() || !decision.reason[0];
    }
    Check(!anyReported,
          "DXVK keeps its vendor with DxvkAmdVendor=0, with Multisampling=0, with the system d3d9.dll, with a card "
          "that is not NVIDIA and when DXVK_CONFIG already sets d3d9.customVendorId, each with its reason");

    Check(PciDeviceIdIsNvidia("PCI\\VEN_10DE&DEV_2882&SUBSYS_00000000&REV_A1") &&
              PciDeviceIdIsNvidia("pci\\ven_10de&dev_1f08") && !PciDeviceIdIsNvidia("PCI\\VEN_1002&DEV_163F") &&
              !PciDeviceIdIsNvidia("PCI\\VEN_8086&DEV_A780") && !PciDeviceIdIsNvidia("") &&
              !PciDeviceIdIsNvidia(nullptr),
          "the desktop's graphics card is NVIDIA when its PCI device id names vendor 10DE");
}
}
