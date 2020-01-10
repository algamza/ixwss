/**
 * @file IxThread.h
 * @author Group(SW_Browser) <gsw_browser@humaxdigital.com>
 * @brief It is a base class for creating thread objects.
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

#ifndef __IXS_THREAD__
#define __IXS_THREAD__

namespace std {
    class thread;
}

namespace ixs {

/**
 * @ingroup IXS
 * @{
 */

/**
 * @brief A base class for a thread.
 */ 
class IxThread
{
public:

    /**
     * @brief Constructor for IxThread.  
     */
    IxThread();

    /**
     * @brief Destructor for IxThread.  
     */
    virtual ~IxThread();

    /**
     * @brief Start the thread.
	 * @param[in] msec The thread's sleep value. ( milliseconds )
     * @return True if successful, False if failed.
     */
    bool start(int msec);

    /**
     * @brief Stop the thread.
     * @return True if successful, False if failed.
     */
    bool stop(); 

protected:

    /**
     * @brief It is called with the sleep interval set by the thread.
     */
    virtual void run() = 0;

private:
    std::thread *m_thread;
    int m_msec;
    bool m_exit;
};

/** @} */ // End of doxygen group

}  /* namespace ixs */

#endif /* __IXS_THREAD__ */