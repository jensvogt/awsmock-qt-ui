//
// Created by vogje01 on 5/5/26.
//

#ifndef AWSMOCK_QT_UI_IMAGE_VIEWER_H
#define AWSMOCK_QT_UI_IMAGE_VIEWER_H

//Qt includes
#include <QWidget>

namespace Awsmock::Components {

    QT_BEGIN_NAMESPACE

    namespace Ui {
        class ImageViewer;
    }

    QT_END_NAMESPACE

    class ImageViewer : public QWidget {
        Q_OBJECT

    public:
        /**
         * @brief Constructor
         *
         * @param parent parent widget
         */
        explicit ImageViewer(QWidget *parent = nullptr);

        /**
         * @brief Destructor
         */
        ~ImageViewer() override;

        /**
         * @brief Load an image
         *
         * @param image image pixmap
         */
        void LoadImage(const QPixmap &image) const;

    private:
        /**
         *  @brief UI components
         */
        Ui::ImageViewer *_ui;
    };
} // Awsmock::Components

#endif //AWSMOCK_QT_UI_IMAGEVIEWER_H
