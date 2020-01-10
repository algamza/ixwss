/**
 * @file IXClientThread.h
 * @author Group(SW_Browser) <gsw_browser@humaxdigital.com>
 * @brief Thread tools
 * 
 * (c) 2017 Humax Co., Ltd.
 * This program is produced by Humax Co., Ltd. ("Humax") and
 * the proprietary Software of Humax and its licensors. Humax provides you, as an Authorized Licensee,
 * non-assignable, non-transferable and non-exclusive license to use this Software.
 * You acknowledge that this Software contains valuable trade secrets of Humax and by using this Software
 * you agree to the responsibility to take all reasonable efforts to protect the any information
 * you receive from Humax. You are not permitted to duplicate, modify, distribute, sell or lease and
 * reverse engineer or extract the source code of this Software unless you have Humax's written permission to do so.
 * If you have no authorized license, discontinue using this Software immediately.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND HUMAX MAKES NO PROMISES, REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS,
 * IMPLIED OR STATUTORY, OR OTHERWISE, WITH RESPECT TO THE SOFTWARE.
 * IN NO EVENT SHALL HUMAX BE LIABLE FOR LOST PROFITS, REVENUES, OR DATA, FINANCIAL LOSSES OR INDIRECT, SPECIAL,
 * CONSEQUENTIAL, EXEMPLARTY OR PUNITIVE DAMAGES WHATSOEVER RELATING TO YOUR USE OR INABILITY TO USE THE SOFTWARE.

 * This License is effective until terminated. You may terminate this License at any time by destroying all copies
 * of the Software including all documentation. This License will terminate immediately without notice from Humax
 * to you if you fail to comply with any provision of this License. Upon termination, you must destroy all copies
 * of the Software and all documentation.

 * The laws of the Republic of Korea will apply to any disputes arising out of or relating to this Copyright Notice.
 * All claims arising out of or relating to this Copyright Notice will be litigated in the Seoul Central District Court,
 * in the Republic of Korea.
*/

#ifndef __IX_WSC_THREAD__
#define __IX_WSC_THREAD__

#include <pthread.h>

namespace ixwss {

/**
 * @ingroup IXWSS
 * @{
 */

/**
 * @brief Runnable class to use in the thread class.
 */
class IXClientRunnable {
public:
    friend class IXClientThread;

    /**
     * @brief Constructor for IXClientRunnable. 
     */
    IXClientRunnable() {}

    /**
     * @brief Destructor for IXClientRunnable. 
     */
    virtual ~IXClientRunnable() {}

protected:
    virtual void *run() { return nullptr; }
};

/**
 * @brief Thread class.
 */
class IXClientThread : public IXClientRunnable {
public:
    /**
     * @brief causes the calling thread to sleep either \n
     * until the number of real-time seconds specified in seconds have elapsed \n
     * or until a signal arrives which is not ignored. \n
     * @param[in] msec The number of milliseconds.
     */
    static void sleep(unsigned int msec);

    /**
     * @brief Constructor for IXClientThread. 
     */
    IXClientThread();

    /**
     * @brief Constructor for IXClientThread. 
     * @param[in] name The name of thread you want.
     */
    IXClientThread(const char *name);

    /**
     * @brief Constructor for IXClientThread. 
     * @param[in] thread The reference thread to copy.
     */
    IXClientThread(IXClientThread &thread);

    /**
     * @brief Destructor for IXClientThread. 
     */
    virtual ~IXClientThread();

    /**
     * @brief Assigns a new value to the thread, replacing its current contents.
     * @param[in] th The reference thread to copy.
     * @return The reference of IXClientThread.
     */
    IXClientThread& operator=(IXClientThread& th);

    /**
     * @brief Start the thread.
     * @return return true if it is successful, else return false.
     */
    bool start();

    /**
     * @brief Stop the thread.
     * @return return true if it is successful, else return false.
     */
    bool stop();

    /**
     * @brief Suspend execution of the calling thread until the target thread terminates.
     * @return return true if it is successful, else return false.
     */
    bool join();

    /**
     * @brief Check if the thread is alive.
     * @return return true if The thread is alive, else return false.
     */
    bool isAlive();

    /**
     * @brief Gets the name of thread.
     * @return return the name of thread if the thread has the name, else return nullptr.
     */
    const char *getName() const;

    /**
     * @brief Sets the name of thread.
     * @param[in] name The string value to assign as the name to the Thread.
     */
    void setName(const char *name); 

protected:
    static void *wrapperThread(void *args);
    bool __start(IXClientRunnable *runnable);
private:
    enum { IXClientThread_NAME_LEN = 32 };
protected:
    char m_name[IXClientThread_NAME_LEN];
    pthread_t m_thread;
    bool m_is_alive;
    pthread_mutex_t m_mutex;
};

/** @} */ // End of doxygen group

}  /* namespace ixwss */

#endif /* __IX_WSS_THREAD__ */
