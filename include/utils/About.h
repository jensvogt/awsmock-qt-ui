//
// Created by vogje01 on 11/16/25.
//

#ifndef AWSMOCK_QT_UI_ABOUT_H
#define AWSMOCK_QT_UI_ABOUT_H

#include <QDialog>

#include <utils/Configuration.h>

#include <modules/infrastructure/InfrastructureService.h>
#include <Version.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class About;
}

QT_END_NAMESPACE

class About final : public QDialog {
    Q_OBJECT

public:
    explicit About(QWidget *parent = nullptr);

    ~About() override;

    void UpdateServerConfig(const GatewayConfig &gatewayConfig) const;

    void UpdateServiceConfig(const GatewayConfig &gatewayConfig) const;

private:
    Ui::About *_ui;

    InfraStructureService *_infraStructureService;
};


#endif //AWSMOCK_QT_UI_ABOUT_H
