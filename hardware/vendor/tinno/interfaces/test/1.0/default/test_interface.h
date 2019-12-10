/*
 * Copyright (C) 2013 The Android Open Source Project
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

#ifndef _HARDWARE_TEST_H
#define _HARDWARE_TEST_H

#include <hardware/hardware.h>
#include <log/log.h>

__BEGIN_DECLS

#define TEST_API_VERSION HARDWARE_MODULE_API_VERSION(1,0)

/**
 * The id of this module
 */
#define TEST_HARDWARE_MODULE_ID "test"

/**
 * The id of the main vibrator device
 */
#define TEST_DEVICE_ID_MAIN "main_test"

struct test_device;
typedef struct test_device {
    /**
     * Common methods of the vibrator device.  This *must* be the first member of
     * vibrator_device as users of this structure will cast a hw_device_t to
     * vibrator_device pointer in contexts where it's known the hw_device_t references a
     * vibrator_device.
     */
    struct hw_device_t common;

    /** Turn on vibrator
     *
     * This function must only be called after the previous timeout has expired or
     * was canceled (through vibrator_off()).
     *
     * @param timeout_ms number of milliseconds to vibrate
     *
     * @return 0 in case of success, negative errno code else
     */
    int (*vibrator_on)(struct test_device* testdev);

    /** Turn off vibrator
     *
     * Cancel a previously-started vibration, if any.
     *
     * @return 0 in case of success, negative errno code else
     */
    int (*vibrator_off)(struct test_device* testdev);
} test_device_t;

static inline int test_open(const struct hw_module_t* module, test_device_t** device)
{
    return module->methods->open(module, TEST_DEVICE_ID_MAIN, TO_HW_DEVICE_T_OPEN(device));
}

__END_DECLS

#endif  // _HARDWARE_VIBRATOR_H
