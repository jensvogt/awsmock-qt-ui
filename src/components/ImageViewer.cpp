//
// Created by vogje01 on 5/5/26.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ImageViewer.h" resolved

#include <components/ImageViewer.h>
#include "ui_ImageViewer.h"

namespace Awsmock::Components {

    ImageViewer::ImageViewer(QWidget *parent) : QWidget(parent), _ui(new Ui::ImageViewer) {
        _ui->setupUi(this);
    }

    ImageViewer::~ImageViewer() {
        delete _ui;
    }

    void ImageViewer::LoadImage(const QPixmap &image) const {
        _ui->imageLabel->setPixmap(image.scaled(_ui->imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        _ui->imageLabel->setScaledContents(false);
        _ui->imageLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        _ui->imageLabel->show();
    }
} // Awsmock::Components
