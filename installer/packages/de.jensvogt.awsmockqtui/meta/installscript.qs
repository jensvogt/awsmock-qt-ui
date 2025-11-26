function Component() {}

Component.prototype.install = function() {
    installer.performOperation("CreateShortcut",
                               "@TargetDir@/AwsMockQtUi.exe",
                               "@StartMenuDir@/AwsMockQtUi.lnk");
}

Component.prototype.uninstall = function() {
    // Optional cleanup
}