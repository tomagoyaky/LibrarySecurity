/*
 * CheckJDWP.h
 *
 *  Created on: 2016年1月26日
 *      Author: peng
 */

#ifndef MODEL_CHECKJDWP_H_
#define MODEL_CHECKJDWP_H_

namespace CheckJDWP{

	/**
     * This is the pathname to the sysfs file that enables and disables
     * tracing on the qemu emulator.
     */
	#define SYSFS_QEMU_TRACE_STATE "/sys/qemu_trace/state";
//  >>> Dalvik
//  xref: /dalvik/vm/jdwp/Jdwp.h
//	bool dvmJdwpCheckConnection(JdwpState* state);
//	bool dvmJdwpAcceptConnection(JdwpState* state);
//	bool dvmJdwpEstablishConnection(JdwpState* state);
//	void dvmJdwpCloseConnection(JdwpState* state);

//  >>> ART
/*
 * Returns "true" if a debugger or DDM is connected.
 */
//	bool IsActive();

	bool isHaveJDWPConnect();
}

#endif /* MODEL_CHECKJDWP_H_ */
