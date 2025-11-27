#ifndef AWSMOCK_QT_UI_S3_SERVICE_H
#define AWSMOCK_QT_UI_S3_SERVICE_H

#include <QMessageBox>
#include <QObject>
#include <QUrlQuery>

#include <utils/Configuration.h>
#include <utils/RestManager.h>
#include <dto/s3/S3ListBucketResult.h>
#include <dto/s3/S3GetBucketDetailsResponse.h>
#include <dto/s3/S3GetObjectDetailsResponse.h>
#include <dto/s3/S3ListObjectResponse.h>

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

    void ListObjects(const QString &bucketName, const QString &prefix);

    void GetObjectDetails(const QString &objectId);

    void DeleteObject(const QString &bucketName, const QString &key);

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

    /**
     * @brief List S3 object signal
     *
     * @param response
     */
    void ListObjectsSignal(const S3ListObjectsResponse &response);

    /**
     * @brief Signaled when an object is available
     *
     * @param objectDetailsResponse object details
     */
    void GetObjectDetailsSignal(const S3GetObjectDetailsResponse &objectDetailsResponse);

    /**
     * @brief Reload object list signal
     */
    void ReloadObjectsSignal();

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
