/*
 * Copyright (C) 2019 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "MyTest"

#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>
#include <binder/ProcessState.h>

#include <gui/BufferQueue.h>

#include <thread>

using namespace android;

struct DummyConsumer : public BnConsumerListener {
    void onFrameAvailable(const BufferItem& /* item */) override {}
    void onBuffersReleased() override {}
    void onSidebandStreamChanged() override {}
};

int main(int argc, char** argv)
{
    sp<IGraphicBufferProducer> producer;
    sp<IGraphicBufferConsumer> consumer;
    BufferQueue::createBufferQueue(&producer, &consumer);

    consumer->consumerConnect(new DummyConsumer, false);

    sp<IServiceManager> serviceManager = defaultServiceManager();
    serviceManager->addService(String16("MyProducer"), IInterface::asBinder(producer));
    serviceManager->addService(String16("MyConsumer"), IInterface::asBinder(consumer));

    ProcessState::self()->startThreadPool();
    IPCThreadState::self()->joinThreadPool();

    return 0;
}
