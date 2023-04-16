/**
 ******************************************************************************
 * @file    ITS_BRD_conf.h
 * @author  Tobias De  Gasperis
 *          HAW-Hamburg
 *          Labor für technische Informatik
 *          Berliner Tor  7
 *          D-20099 Hamburg
 * @version 1.0
 *
 * @date    22. April 2022
 * @brief   Definitionen fuer die Configuration des USART3 fuer stdin und stdout
 *
 ******************************************************************************
 */
 

#ifndef _ITS_BRD_CONF_H
#define	_ITS_BRD_CONF_H
	
	#define RTE_DEVICE_FRAMEWORK_CLASSIC
	#define RTE_Compiler_IO_STDIN_User
	#define RTE_Compiler_IO_STDIN
	#define RTE_Compiler_IO_STDOUT_User
	#define RTE_Compiler_IO_STDOUT

#endif
