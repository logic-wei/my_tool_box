# ndk-stack-asan

Add ASan support for Android ndk-stack.

## Install

Move the ndk-stack-asan.py to where the ndk-stack.py is.

```bash
mv ndk-stack-asan.py $NDK/prebuilt/linux-x86_64/bin
```

Create a symbolic link in `/usr/local/bin`.

```bash
sudo ln -s $NDK/prebuilt/linux-x86_64/bin/ndk-stack-asan.py /usr/local/bin/ndk-stack-asan
```

## Usage

Click the command just like this:
```bash
ndk-stack-asan -sym $SYMBOLS_PATH -i $LOG_PATH > parsed_backtrace.txt
```
And then you can get the result parsed_bakctrace.txt like this:

```bash
...
##############################ASAN BEGIN##############################
12-06 11:44:09.069 13850 14152 I android.hardware.camera.provider@2.4-service_64: READ of size 65496 at 0x00762a6ec800 thread T17
12-06 11:44:09.086 13850 14152 I android.hardware.camera.provider@2.4-service_64:     #0 0x773e78354c  (/system/lib64/libclang_rt.asan-aarch64-android.so+0xc254c)
12-06 11:44:09.087 13850 14152 I android.hardware.camera.provider@2.4-service_64:     #1 0x7735c3a6a8  (/vendor/lib64/hw/camera.qcom.so+0x8b46a8)
--------------------------------------------------------------------------------------------------------------------------------------------------------------------
-->       CamX::Utils::Memcpy(void*, void const*, unsigned long)        <--
-->       out/target/product/umi/obj/include/camx/camxutils.h:1989:16        <--
-->       CamX::JPEGEXIFComposer::EmitNBytes(unsigned char const*, unsigned int, unsigned char*, unsigned int*, unsigned int, int, unsigned int)        <--
-->       vendor/qcom/proprietary/camx/src/swl/jpeg/camxjpegexifcomposer.cpp:2110:0        <--

12-06 11:44:09.087 13850 14152 I android.hardware.camera.provider@2.4-service_64:     #2 0x7735c3a0f8  (/vendor/lib64/hw/camera.qcom.so+0x8b40f8)
--------------------------------------------------------------------------------------------------------------------------------------------------------------------
-->       CamX::JPEGEXIFComposer::EmitAppByType(CamX::ExifPayloadType, unsigned char const*, unsigned int, unsigned int, unsigned char)        <--
-->       vendor/qcom/proprietary/camx/src/swl/jpeg/camxjpegexifcomposer.cpp:2301:18        <--

12-06 11:44:09.087 13850 14152 I android.hardware.camera.provider@2.4-service_64:     #3 0x7735c2fb54  (/vendor/lib64/hw/camera.qcom.so+0x8a9b54)
--------------------------------------------------------------------------------------------------------------------------------------------------------------------
-->       CamX::JPEGEXIFComposer::FlushFileHeader()        <--
-->       vendor/qcom/proprietary/camx/src/swl/jpeg/camxjpegexifcomposer.cpp:1199:13        <--

12-06 11:44:09.087 13850 14152 I android.hardware.camera.provider@2.4-service_64:     #4 0x7735c2c508  (/vendor/lib64/hw/camera.qcom.so+0x8a6508)
--------------------------------------------------------------------------------------------------------------------------------------------------------------------
-->       CamX::JPEGEXIFComposer::EmitEXIF()        <--
-->       vendor/qcom/proprietary/camx/src/swl/jpeg/camxjpegexifcomposer.cpp:408:18        <--

12-06 11:44:09.087 13850 14152 I android.hardware.camera.provider@2.4-service_64:     #5 0x7735c2af34  (/vendor/lib64/hw/camera.qcom.so+0x8a4f34)
--------------------------------------------------------------------------------------------------------------------------------------------------------------------
-->       CamX::JPEGEXIFComposer::StartComposition()        <--
-->       vendor/qcom/proprietary/camx/src/swl/jpeg/camxjpegexifcomposer.cpp:266:18        <--

12-06 11:44:09.087 13850 14152 I android.hardware.camera.provider@2.4-service_64:     #6 0x7735c27528  (/vendor/lib64/hw/camera.qcom.so+0x8a1528)
--------------------------------------------------------------------------------------------------------------------------------------------------------------------
-->       CamX::JPEGAggrNode::ExecuteProcessRequest(CamX::ExecuteProcessRequestData*)        <--
-->       vendor/qcom/proprietary/camx/src/swl/jpeg/camxjpegaggrnode.cpp:781:39        <--

12-06 11:44:09.087 13850 14152 I android.hardware.camera.provider@2.4-service_64:     #7 0x77363faf34  (/vendor/lib64/hw/camera.qcom.so+0x1074f34)
--------------------------------------------------------------------------------------------------------------------------------------------------------------------
-->       CamX::Node::ProcessRequest(CamX::NodeProcessRequestData*, unsigned long)        <--
-->       vendor/qcom/proprietary/camx/src/core/camxnode.cpp:1768:26        <--

12-06 11:44:09.087 13850 14152 I android.hardware.camera.provider@2.4-service_64:     #8 0x77362feb6c  (/vendor/lib64/hw/camera.qcom.so+0xf78b6c)
--------------------------------------------------------------------------------------------------------------------------------------------------------------------
-->       CamX::DeferredRequestQueue::DeferredWorkerCore(CamX::Dependency*)        <--
-->       vendor/qcom/proprietary/camx/src/core/camxdeferredrequestqueue.cpp:292:25        <--
...
```
