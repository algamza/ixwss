/**
 * @file IXThread.cpp
 * @author Group(SW_Browser) <gsw_browser@humaxdigital.com>
 * @brief Thread of IX
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


#include "IXThread.h"

#include <cstddef>
#include <unistd.h>
#include <string.h>
#include <cstdio>
#include <iostream>

using namespace std;

namespace ixwss {

typedef struct _hwssth_args {
	IXThread	*thiz;
	IXRunnable *runnable;
} hwssth_arg;

/*---------------------------------------------------------------
 * IXThread
 *--------------------------------------------------------------*/
IXThread::IXThread()
	: m_thread(0), m_is_alive(false)
{
	m_name[0] = '\0';
}

IXThread::IXThread(const char *name)
	: m_thread(0), m_is_alive(false)
{
	setName(name);
}

IXThread::IXThread(IXThread &runnable)
	: m_thread(0), m_is_alive(false)
{
	m_name[0] = '\0';
}

IXThread::~IXThread()
{
	stop();
}

IXThread& IXThread::operator=(IXThread& th)
{
	stop();

	setName(th.getName());
	__start(&th);

	return *this;
}

bool IXThread::start()
{
	return __start(this);
}

bool IXThread::stop()
{
	if ( m_thread == 0 ) return false;

	if ( pthread_self() == m_thread )
	{
		pthread_exit(PTHREAD_CANCELED);
	}
	else
	{
		if ( pthread_cancel(m_thread) != 0 ) return false;
	}

	m_is_alive = false;
	m_thread = 0;

	return true;
}

bool IXThread::join()
{
	int status = 0;
	return pthread_join(m_thread, reinterpret_cast<void **>(&status)) == 0;
}

bool IXThread::isAlive()
{
	return m_is_alive;
}

const char *IXThread::getName() const
{
	return m_name;
}

void IXThread::setName(const char *name)
{
	if ( name )
	{	
		strncpy(this->m_name, name, IXTHREAD_NAME_LEN);
	}
}

void IXThread::sleep(unsigned int msec)
{
	usleep(msec*1000);
}

bool IXThread::__start(IXRunnable *runnable)
{
	int r = pthread_mutex_init(&m_mutex, nullptr);
	if ( r != 0 )
	{
		return false;
	}

	hwssth_arg arg = { this, runnable };
	r = pthread_create(&m_thread, nullptr, wrapperThread, &arg);
	if ( r != 0 )
	{
		pthread_mutex_destroy(&m_mutex);
		return false;
	}

	// wait until the thread is initialized.
	pthread_mutex_lock(&m_mutex);
	pthread_mutex_lock(&m_mutex);

	pthread_mutex_destroy(&m_mutex);

	return true;
}

void *IXThread::wrapperThread(void *args)
{
	hwssth_arg *hwssarg = static_cast<hwssth_arg *>(args);
	if ( !hwssarg ) return nullptr;

	IXThread *thiz = hwssarg->thiz;
	if ( !thiz ) return nullptr;

	IXRunnable *runnable = hwssarg->runnable;
	if ( !runnable ) return nullptr;

	thiz->m_is_alive = true;
	pthread_mutex_unlock(&(thiz->m_mutex));

	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, nullptr);

	void *r = runnable->run();
	
	pthread_exit(r);

	return r;
}

}  /* namespace ixwss */