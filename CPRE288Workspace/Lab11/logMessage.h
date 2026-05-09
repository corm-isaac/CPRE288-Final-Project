/*
 * @file logMessage.h
 * @brief print method to uartStr
 * @date 04/10/26
 * @author Isaac Cormier
 */
#ifndef LOGMESSAGE_H_
#define LOGMESSAGE_H_

void log_message(size_t buf_Size, const char *fmt, ...);

void log_message_init(void);

#endif /* LOGMESSAGE_H_ */
