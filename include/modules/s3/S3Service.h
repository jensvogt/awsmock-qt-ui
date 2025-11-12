#ifndef AWSMOCK_QT_UI_S3_SERVICE_H
#define AWSMOCK_QT_UI_S3_SERVICE_H

#include <QMessageBox>
#include <QObject>
#include <QUrlQuery>

#include <utils/Configuration.h>
#include <utils/RestManager.h>
#include <dto/s3/S3ListBucketResult.h>
#include <dto/s3/S3GetBucketDetailsResponse.h>

class S3Service final : public QObject {
    Q_OBJECT

public:
    /**
     * @brief S3 service
     */
    S3Service();

    /**
     * @brief List SNS Topics
     *
     * @param prefix topic name prefix
     */
    void ListBuckets(const QString &prefix);

    /**
     * @brief Purge all objects
     *
     * @param bucketName ARN of the parent bucket
     */
    void PurgeBucket(const QString &bucketName);

    void AddBucket(const QString &bucketName);

    void DeleteBucket(const QString &bucketName);

    void GetBucketDetails(const QString &bucketName);

signals:
    /**
     * @brief Signaled when a bucket is available
     *
     * @param bucketListResponse list of buckets
     */
    void ListBucketSignal(const S3ListBucketResult &bucketListResponse);

    /**
     * @brief Signaled when a bucket list needs to be reloaded
     *
     */
    void ReloadBucketListSignal();

    /**
     * @brief Get bucket details response
     */
    void GetBucketDetailsSignal(const S3GetBucketDetailsResponse &getBucketDetailsResponse);

private:
    /**
     * @brief HTTP REST manager
     */
    RestManager _restManager;

    /**
     * @brief Base URL
     */
    QUrl url;
};


#endif // AWSMOCK_QT_UI_S3_SERVICE_H
