#ifndef AWSMOCK_UI_VERSION_H
#define AWSMOCK_UI_VERSION_H

// These numeric values are automatically updated by release-please
#define AWSMOCK_UI_VERSION_MAJOR 1
#define AWSMOCK_UI_VERSION_MINOR 0
#define AWSMOCK_UI_VERSION_PATCH 1

// This string version is also updated by release-please
#define AWSMOCK_UI_VERSION "1.0.1"

// Optional helpers
#define AWSMOCK_UI_VERSION_STR AWSMOCK_UI_VERSION

#define AWSMOCK_UI_MAKE_VERSION(major, minor, patch) ((major) * 10000 + (minor) * 100 + (patch))

#define AWSMOCK_UI_VERSION_CODE AWSMOCK_UI_MAKE_VERSION(AWSMOCK_UI_VERSION_MAJOR, AWSMOCK_UI_VERSION_MINOR, AWSMOCK_UI_VERSION_PATCH)

#endif // AWSMOCK_UI_VERSION_H
