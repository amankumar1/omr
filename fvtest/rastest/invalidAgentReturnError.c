/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2014, 2015
 *
 *  This program and the accompanying materials are made available
 *  under the terms of the Eclipse Public License v1.0 and
 *  Apache License v2.0 which accompanies this distribution.
 *
 *      The Eclipse Public License is available at
 *      http://www.eclipse.org/legal/epl-v10.html
 *
 *      The Apache License v2.0 is available at
 *      http://www.opensource.org/licenses/apache2.0.php
 *
 * Contributors:
 *    Multiple authors (IBM Corp.) - initial implementation and documentation
 *******************************************************************************/

#include "omragent.h"

/*
 * Invalid Agent - return error
 */

omr_error_t OMRAgent_OnLoad(OMR_TI const *ti, OMR_VM *vm, char const *options,
                            OMR_AgentCallbacks *agentCallbacks, ...) {
  return OMR_ERROR_OUT_OF_NATIVE_MEMORY;
}

omr_error_t OMRAgent_OnUnload(OMR_TI const *ti, OMR_VM *vm) {
  return OMR_ERROR_ILLEGAL_ARGUMENT;
}
