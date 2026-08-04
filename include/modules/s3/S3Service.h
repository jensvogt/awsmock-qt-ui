#pragma once

// Qt includes
#include <QMessageBox>
#include <QElapsedTimer>

// AwsMock includes
#include <utils/Logging.h>
#include <utils/Configuration.h>
#include <utils/RestManager.h>
#include <utils/BaseService.h>
#include <utils/EventBus.h>
#include <dto/s3/S3ListBucketResult.h>
#include <dto/s3/S3GetBucketDetailsResponse.h>
#include <dto/s3/S3GetObjectDetailsResponse.h>
#include <dto/s3/S3ListObjectResponse.h>
#include <dto/s3/S3ListObjectVersionCountersResponse.h>
#include <dto/s3/S3PutBucketNotificationConfigurationRequest.h>

class S3Service final : public BaseService {
    Q_OBJECT

public:
    /**
     * @brief S3 service
     */
    S3Service() {
        setApis({
            "ListBuckets", "ListBucketArns", "PurgeBucket", "PurgeAllBuckets", "AddBucket", "UpdateBucket", "ResetObjectCounters", "DeleteBucket",
            "GetBucketDetails", "ListObjects", "GetObjectDetails", "UploadObject", "UpdateObject", "PutBucketNotificationConfiguration",
            "TouchObject", "DeleteObject", "ListObjectVersionCounters"
        });
    }

    /**
     * @brief List of S3 buckets
     *
     * @param prefix topic name prefix
     * @param pageSize page size
     * @param pageIndex page index
     * @param sortAttribute sort database attribute name
     * @param sortDirection sort direction 1=ascending, -1=descending
     */
    void ListBuckets(const QString &prefix, long pageSize, long pageIndex, const QString &sortAttribute, int sortDirection);

    /**
     * @brief List S3 bucket ARNs
     */
    void ListBucketArns();

    /**
     * @brief Purge all objects
     *
     * @param bucketName ARN of the parent bucket
     */
    void PurgeBucket(const QString &bucketName);

    /**
     * @brief Purge all buckets
     */
    void PurgeAllBuckets();

    /**
     * @brief Add a new bucket
     *
     * @param bucketName name of the bucket
     */
    void AddBucket(const QString &bucketName);

    /**
     * @brief Update bucket
     *
     * @param bucketName bucket name
     * @param metadata bucket metadata array
     * @param lifecycleRules
     * @param versionStatus version status, can be either 'enabled' or 'disabled'.
     */
    void UpdateBucket(const QString &bucketName, QMap<QString, QString> &metadata, QVector<LifecycleRule> &lifecycleRules, const QString &versionStatus);

    /**
     * @brief Reset all bucket counters
     */
    void ResetObjectCounters();

    /**
     * @brief delete the bucket
     *
     * @param bucketName name of the bucket
     */
    void DeleteBucket(const QString &bucketName);

    /**
     * @brief Get the bucket details
     *
     * @param bucketName name of the bucket
     */
    void GetBucketDetails(const QString &bucketName);

    /**
     * @brief List all object of a bucket.
     *
     * @param bucketName name of the bucket
     * @param prefix bucket name prefix
     * @param pageSize page size
     * @param pageIndex page index
     * @param sortAttribute sort attribute
     * @param sortDirection sort direction
     */
    void ListObjects(const QString &bucketName, const QString &prefix, long pageSize, long pageIndex, const QString &sortAttribute, int sortDirection);

    /**
     * @brief Retrieves the details of an S3 object.
     *
     * This method sends an asynchronous request to fetch the details of a specific
     * object in an S3 bucket. Upon successful response, it processes the received
     * JSON data and emits a signal with the parsed object details. In case of a failure,
     * the error is logged. Additionally, a timer signal is emitted to measure the duration
     * of the operation.
     *
     * @param objectId The unique identifier of the S3 object.
     */
    void GetObjectDetails(const QString &objectId);

    /**
     * @brief Lists all stored versions of a single object key, most recently modified first.
     *
     * @param bucketName name of the bucket
     * @param key object key
     */
    void ListObjectVersionCounters(const QString &bucketName, const QString &key);

    /**
     * @brief Uploads an object to the specified S3 bucket with metadata and content.
     *
     * @param bucketName The name of the S3 bucket to which the object will be uploaded.
     * @param bucketArn The ARN of the S3 bucket.
     * @param key The key (name) of the object to be uploaded.
     * @param content The content of the object, represented as a QByteArray.
     * @param metadata A map of metadata key-value pairs associated with the object.
     */
    void UploadObject(const QString &bucketName, const QString &bucketArn, const QString &key, const QByteArray &content, const QString &contentType, const QMap<QString, QString> &metadata);

    /**
     * @brief Updates an object in an S3 bucket.
     *
     * @param region The region where the S3 bucket is located.
     * @param bucketName The name of the S3 bucket.
     * @param key The key of the object to be updated.
     * @param content The content to update the object with, encoded as a QByteArray.
     * @param storageClass The storage class for the updated object (e.g., STANDARD, REDUCED_REDUNDANCY).
     * @param metadata A map of metadata key-value pairs to set for the object.
     */
    void UpdateObject(const QString &region, const QString &bucketName, const QString &key, const QByteArray &content, const QString &storageClass, const QMap<QString, QString> &metadata);

    /**
     * @brief Configures the notification settings for an S3 bucket by sending a request to the S3 service.
     *
     * @param request The request object containing the bucket name and notification configuration details in XML format.
     */
    void PutBucketNotificationConfiguration(const S3PutBucketNotificationConfigurationRequest &request);

    /**
     * @brief Puts the full notification configuration for a bucket from individual notification lists.
     *
     * @param bucket The name of the S3 bucket.
     * @param lambdaNotifications List of lambda notification configurations.
     * @param queueNotifications List of SQS queue notification configurations.
     * @param topicNotifications List of SNS topic notification configurations.
     */
    void PutBucketNotificationConfiguration(const QString &bucket,
                                            const QList<LambdaNotification> &lambdaNotifications,
                                            const QList<QueueNotification> &queueNotifications,
                                            const QList<TopicNotification> &topicNotifications);

    /**
     * @brief Updates the timestamp or metadata of an object in an S3-compatible storage bucket.
     *
     * This method sends a request to simulate touching an object within a bucket, potentially
     * modifying its metadata to indicate a recent interaction. Upon success, it triggers
     * the ReloadObjectsSignal to refresh the stored object list.
     *
     * @param bucketName The name of the bucket containing the object to touch.
     * @param key The key identifying the object within the bucket.
     */
    void TouchObject(const QString &bucketName, const QString &key);

    /**
     * @brief Deletes an object from the specified S3 bucket.
     *
     * This method sends a request to remove an object with the specified key
     * from the given bucket. Upon success or failure, appropriate signals
     * are emitted for further handling.
     *
     * @param bucketName The name of the S3 bucket from which the object is to be deleted.
     * @param key The key identifying the object to delete within the bucket.
     */
    void DeleteObject(const QString &bucketName, const QString &key);

signals
:
    /**
     * @brief Signaled when a bucket is available
     *
     * @param bucketListResponse list of buckets
     */
    void ListBucketSignal(const S3ListBucketResult &bucketListResponse);

    /**
     * @brief List S3 bucket ARNs signal
     *
     * @param arns list of bucket ARNs
     */
    void ListBucketArnsSignal(const QList<QString> &arns);

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
     * @brief Signaled when the list of object versions is available
     *
     * @param versionCountersResponse list of object versions
     */
    void ListObjectVersionCountersSignal(const S3ListObjectVersionCountersResponse &versionCountersResponse);

    /**
     * @brief Reload object list signal
     */
    void ReloadObjectsSignal();

private:
    /**
     * @brief HTTP REST manager
     */
    RestManager _restManager;
};

