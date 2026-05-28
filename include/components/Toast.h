//
// Created by jensv on 22/04/2026.
//

#pragma once

// Qt includes
#include <QGraphicsOpacityEffect>
#include <QLabel>
#include <QPropertyAnimation>
#include <QTimer>

namespace Awsmock::Components {

    class ToastOverlay : public QLabel {
        Q_OBJECT

    public:
        explicit ToastOverlay(const QString &message, QWidget *parent = nullptr, const int durationMs = 3000) : QLabel(message, parent) {

            // Styling
            setStyleSheet(R"(
            background-color: rgba(50, 50, 50, 220);
            color: white;
            border-radius: 8px;
            padding: 10px 20px;
            font-size: 13px;
        )");
            setAlignment(Qt::AlignTop | Qt::AlignHCenter);
            adjustSize();

            // Float above everything
            raise();
            reposition();
            show();

            // Fade out then delete
            QTimer::singleShot(durationMs, this, [this]() { fadeOut(); });
        }

        void reposition() {
            if (!parentWidget()) return;
            const QSize ps = parentWidget()->size();
            move((ps.width() - width()) / 2, (ps.height() - height()) / 2);
        }

    private:
        void fadeOut() {
            auto *effect = new QGraphicsOpacityEffect(this);
            setGraphicsEffect(effect);

            auto *anim = new QPropertyAnimation(effect, "opacity", this);
            anim->setDuration(400);
            anim->setStartValue(1.0);
            anim->setEndValue(0.0);
            anim->setEasingCurve(QEasingCurve::OutCubic);
            anim->start();

            connect(anim, &QPropertyAnimation::finished, this, &QObject::deleteLater);
        }
    };

}
