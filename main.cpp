#include <iostream>
#include <fstream>

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

#include <aws/core/utils/stream/ResponseStream.h>

void TestDynamoDB();
void TestPolly();
int GetStreamSize(Aws::IOStream* stream);
char* GetStreamBytes(Aws::IOStream* stream);

int main(int argc, char *argv[])
{
    printf("Hello! This is a quick example on how to use the AWS C++ SDK.\n");

    // Initialize the SDK. You'll get a runtime error without this!
    Aws::SDKOptions options;
    Aws::InitAPI(options);

    TestPolly();

    Aws::ShutdownAPI(options);

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
        std::cout << "PutItem Success Using IOPS " << putItemOutcome.GetResult().GetConsumedCapacity().GetTableName() << "\n";
    }
    else{
        std::cout << "PutItem failed with error " << putItemOutcome.GetError().GetMessage() << "\n";
    }
}

void TestPolly()
{
    Aws::Polly::PollyClient pollyClient;
    Aws::Polly::Model::SynthesizeSpeechRequest speechRequest;

    speechRequest.SetVoiceId(Aws::Polly::Model::VoiceId::Kendra);
    speechRequest.SetOutputFormat(Aws::Polly::Model::OutputFormat::mp3);

    speechRequest.SetText("Flight 1 2 7 is now headed to Denver. Please make appropriate adjustments.");
    auto result = pollyClient.SynthesizeSpeech(speechRequest);

    if (result.IsSuccess()){
        std::cout << "Speech synthesis was successfull. Saving to file now..." << std::endl;

        Aws::String     file_name = "polly.mp3";
        Aws::IOStream*  audio_stream = &result.GetResult().GetAudioStream();
        Aws::OFStream   voice_file;

        // Get the bytes from the stream and save them into a file
        voice_file.open(file_name.c_str(), std::ios::out | std::ios::binary);
        voice_file.write(GetStreamBytes(audio_stream), GetStreamSize(audio_stream));
        voice_file.close();

        std::cout << "Done!" << std::endl;
    }
    else{
        std::cout << "Speech synthesis failed.\n";
        std::cout << "Error: " << result.GetError().GetMessage();
    }
}

int GetStreamSize(Aws::IOStream* stream){
    // Ensure the stream is at the beginning
                stream->seekg (0, std::ios::beg);
    int begin = stream->tellg();
                stream->seekg (0, std::ios::end);
    int end =   stream->tellg();
                stream->seekg (0, std::ios::beg);

    return end-begin;
}

char* GetStreamBytes(Aws::IOStream* stream){
    // Read the stream bytes into a memory block
    int size = GetStreamSize(stream);
    char* bytes = new char[size];
    stream->read(bytes, size);

    return bytes;
}
