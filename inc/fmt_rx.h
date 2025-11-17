/** Takes care of:
 * - Decoding incoming messages
 * - calling the appropriate handler for the sub-message type.
 * 
 * calls fmt_getMsg
 */
void fmt_handleRx(void);