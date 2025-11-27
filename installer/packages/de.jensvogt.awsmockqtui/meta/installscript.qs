function Component() {}

Component.prototype.install = function() {
    installer.performOperation("CreateShortcut",
                               "@TargetDir@/AwsMockQtUi.exe",
                               "@StartMenuDir@/AwsMockQtUi.lnk");
}

Component.prototype.uninstall = function() {
    // Optional cleanup
}

Component.prototype.createOperations = function() {
    component.createOperations();
    component.addOperation("CreateDesktopShortcut", "@TargetDir@/awsmock-qt-ui.exe", "AwsmockQtUi");
}