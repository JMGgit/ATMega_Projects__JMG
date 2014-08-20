/*
 * Modes_All.h
 *
 *  Created on: 21.08.2014
 *      Author: JMG
 */

#ifndef MODES_ALL_H_
#define MODES_ALL_H_

#if (PROJECT == PROJECT__TEMPERATURE_LOGGER)
#include "../Modes_TemperatureLogger/Modes.h"
#endif

#if (PROJECT == PROJECT__QLOCKTWO)
#include "../Modes_Qlocktwo/Modes.h"
#endif

#if (PROJECT == PROJECT__LED_TABLE)
#include "../Modes_LedTable/Modes.h"
#endif


#endif /* MODES_ALLH_ */
