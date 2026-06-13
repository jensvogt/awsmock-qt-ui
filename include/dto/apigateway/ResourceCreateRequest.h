//
// Created by vogje01 on 6/13/26.
//

#pragma once

// Qt include
#include <QString>

struct ResourceCreateRequest {

    /**
     * ID of the corresponding REST API
     */
    QString restApiId;

    /**
     * Parent ID
     */
    QString parentId;

    /**
     * Path part
     */
    QString pathPart;
};
