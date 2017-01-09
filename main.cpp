#include <iostream>

// AWS Core includes
#include <aws/core/Aws.h>
#include <aws/core/auth/AWSCredentialsProviderChain.h>
#include <aws/core/client/AWSClient.h>
#include <aws/core/utils/Outcome.h>

// AWS DynamoDB includes
#include <aws/dynamodb/DynamoDBClient.h>
#include <aws/dynamodb/model/PutItemRequest.h>
#include <aws/dynamodb/model/PutItemResult.h>
#include <aws/dynamodb/model/ConsumedCapacity.h>

// AWS Polly Includes
#include <aws/polly/PollyClient.h>
#include <aws/polly/model/SynthesizeSpeechRequest.h>
#include <aws/polly/model/SynthesizeSpeechResult.h>
#include <aws/polly/PollyRequest.h>

void TestDynamoDB();
void TestPolly();

using namespace std;

int main(int argc, char *argv[])
{
    printf("Hello! This is a quick example on how to use the AWS C++ SDK.\n");

    // Initialize the SDK. You'll get a runtime error without this!
    Aws::SDKOptions options;
    Aws::InitAPI(options);

    TestPolly();

    return 0;
}

void TestDynamoDB()
{
    // Create clients and necessary variables
    Aws::DynamoDB::DynamoDBClient dynamoDbClient;
    Aws::DynamoDB::Model::PutItemRequest putItemRequest;
    Aws::DynamoDB::Model::AttributeValue hashKeyAttribute, valueAttribute;

    // Create the query to be sent to the database
    putItemRequest.WithTableName("DeveloperTest1");
    hashKeyAttribute.SetS("SampleHashKeyValue");
    putItemRequest.AddItem("HashKey", hashKeyAttribute);
    valueAttribute.SetS("SampleValue");
    putItemRequest.AddItem("Name", valueAttribute);
    auto putItemOutcome = dynamoDbClient.PutItem(putItemRequest);

    // Process the results of the request
    if(putItemOutcome.IsSuccess()){
        cout << "PutItem Success Using IOPS " << putItemOutcome.GetResult().GetConsumedCapacity().GetTableName() << "\n";
    }
    else{
        cout << "PutItem failed with error " << putItemOutcome.GetError().GetMessage() << "\n";
    }
}

void TestPolly()
{
    Aws::Polly::PollyClient pollyClient;
    Aws::Polly::Model::SynthesizeSpeechRequest speechRequest;

    speechRequest.SetVoiceId(Aws::Polly::Model::VoiceId::Kendra);
    speechRequest.SetOutputFormat(Aws::Polly::Model::OutputFormat::mp3);

    speechRequest.SetText("Hello Dave, how are you doing today?");
    auto result = pollyClient.SynthesizeSpeech(speechRequest);

//    Aws::IOStream audioStream = result.GetResult().GetAudioStream();

    if (result.IsSuccess()){
        cout << "Speech synthesis was successfull.";
    }
    else{
        cout << "Speech synthesis failed.\n";
        cout << "Error: " << result.GetError().GetMessage();
    }
}
