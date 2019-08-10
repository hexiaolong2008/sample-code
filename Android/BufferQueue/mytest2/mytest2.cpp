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

#include <gui/BufferItem.h>
#include <gui/BufferQueue.h>
#include <gui/IProducerListener.h>

#include <ui/GraphicBuffer.h>

#include <system/window.h>

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

    IGraphicBufferProducer::QueueBufferOutput output;
    producer->connect(new DummyProducerListener, NATIVE_WINDOW_API_CPU, false, &output);

    /****************** Producer ***********************/
    int slot;
    sp<Fence> fence;
    sp<GraphicBuffer> buffer;
    producer->dequeueBuffer(&slot, &fence, 0, 0, 0,
		    	     GRALLOC_USAGE_SW_WRITE_OFTEN, nullptr, nullptr);
	printf("dequeueBuffer: get slot%d\n", slot);

    producer->requestBuffer(slot, &buffer);
	printf("requestBuffer: slot%d\n", slot);

	producer->cancelBuffer(slot, fence);
	printf("cancelBuffer: slot%d\n", slot);

	producer->attachBuffer(&Slot, buffer);
	printf("attachBuffer: get slot%d\n", slot);

	producer->detachBuffer(slot);
	printf("detachBuffer: slot%d\n", slot);

    producer->dequeueBuffer(&slot, &fence, 0, 0, 0,
		    	     GRALLOC_USAGE_SW_WRITE_OFTEN, nullptr, nullptr);
	printf("dequeueBuffer: get slot%d\n", slot);

    IGraphicBufferProducer::QueueBufferInput input(0, false, HAL_DATASPACE_UNKNOWN, Rect(1, 1),
				    NATIVE_WINDOW_SCALING_MODE_FREEZE, 0, Fence::NO_FENCE);
    producer->queueBuffer(slot, input, &output);
	printf("queueBuffer: slot%d\n", slot);


    /****************** Consumer ***********************/
    BufferItem item;
    consumer->acquireBuffer(&item, static_cast<nsecs_t>(0));
	printf("acquireBuffer: get slot%d\n", slot);

    consumer->releaseBuffer(item.mSlot, item.mFrameNumber, EGL_NO_DISPLAY,
		    	     EGL_NO_SYNC_KHR, Fence::NO_FENCE);
	printf("releaseBuffer: slot%d\n", slot);

	consumer->attachBuffer(&slot, item.mGraphicBuffer);
	printf("attachBuffer: get slot%d\n", slot);

	consumer->detachBuffer(slot);
	printf("detachBuffer: slot%d\n", slot);

	consumer->discardFreeBuffers();
	printf("discardFreeBuffers\n");

    return 0;
}
