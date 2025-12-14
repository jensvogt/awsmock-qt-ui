// packages/your.custom.library/meta/installscript.qs

function Component()
{
    // The constructor is called once per installation run
}

Component.prototype.createOperations = function()
{
    // Call the base implementation first
    component.createOperations();

    var installDir = installer.value("TargetDir"); // The root directory chosen by the user

    // --- 1. Installation of Binaries (DLLs/SOs) ---
    component.addOperation("Copy", "data/bin/libcurl.dll", "@TargetDir@/bin/libcurl.dll");
}