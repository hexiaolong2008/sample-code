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

#include <binder/IServiceManager.h>

#include <gui/BufferItem.h>
#include <gui/BufferQueue.h>
#include <gui/IProducerListener.h>
#include <ui/GraphicBuffer.h>

#include <system/window.h>

using namespace android;

int main(int argc, char** argv)
{
    sp<IServiceManager> serviceManager = defaultServiceManager();
    sp<IBinder> binderProducer = serviceManager->getService(String16("MyProducer"));
    sp<IBinder> binderConsumer = serviceManager->getService(String16("MyConsumer"));
    sp<IGraphicBufferProducer> producer = interface_cast<IGraphicBufferProducer>(binderProducer);
    sp<IGraphicBufferConsumer> consumer = interface_cast<IGraphicBufferConsumer>(binderConsumer);

    IGraphicBufferProducer::QueueBufferOutput output;
    producer->connect(new DummyProducerListener, NATIVE_WINDOW_API_CPU, false, &output);

    /****************** Producer ***********************/
    int slot;
    sp<Fence> fence;
    sp<GraphicBuffer> buffer;
    producer->dequeueBuffer(&slot, &fence, 0, 0, 0,
		    	     GRALLOC_USAGE_SW_WRITE_OFTEN, nullptr, nullptr);
    producer->requestBuffer(slot, &buffer);

    uint32_t* dataIn;
    buffer->lock(GraphicBuffer::USAGE_SW_WRITE_OFTEN,
		 reinterpret_cast<void**>(&dataIn));
    *dataIn = 0x12345678u;
    buffer->unlock();

    IGraphicBufferProducer::QueueBufferInput input(0, false, HAL_DATASPACE_UNKNOWN, Rect(1, 1),
				    NATIVE_WINDOW_SCALING_MODE_FREEZE, 0, Fence::NO_FENCE);
    producer->queueBuffer(slot, input, &output);


    /****************** Consumer ***********************/
    BufferItem item;
    consumer->acquireBuffer(&item, static_cast<nsecs_t>(0));

    uint32_t* dataOut;
    item.mGraphicBuffer->lock(GraphicBuffer::USAGE_SW_READ_OFTEN,
		    	      reinterpret_cast<void**>(&dataOut));
    printf("data out is 0x%08x\n", *dataOut);
    item.mGraphicBuffer->unlock();

    consumer->releaseBuffer(item.mSlot, item.mFrameNumber, EGL_NO_DISPLAY,
		    	     EGL_NO_SYNC_KHR, Fence::NO_FENCE);

    return 0;
}
