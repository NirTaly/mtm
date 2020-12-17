#ifndef INFO_H
#define INFO_H

#include <stdbool.h>

typedef struct Information_t* Info;

typedef struct MemDetails_t* MemDetails;

/**
 * @brief Create a Info object
 * 
 * @param name 
 * @param uid 
 * @param count 
 * @return Info ,NULL on memory fail
 */
Info createInfo(char* name, int uid, unsigned int count);

/**
 * @brief Copy Info object
 * 
 * @return Info, NULL on memory fail
 */
Info copyInfo(Info info);

/**
 * @brief Free Info
 */
void freeInfo(Info info);

/**
* identify equal elements.
* This function should return:
* 		true if they're equal;
*		false otherwise;
*/
bool equalInfo(Info info1, Info info2);

/**
 * @brief Create a Mem object
 * 
 * @param count 
 * @param mem_id 
 * @return MemDetails 
 */
MemDetails createMem(unsigned int count, int mem_id);

/**
 * @brief Copy MemDetails object
 * 
 * @param priority 
 * @return MemDetails , NULL on memory fail
 */
MemDetails copyMemDetails(MemDetails priority);

/**
 * @brief Free MemDetails
 */
void freeMemDetails(MemDetails priority);

/**
* compare priorities.
* This function should return:
* 		A positive integer if the first element is greater;
* 		0 if they're equal;
*		A negative integer if the second element is greater.
*/
int compareMemDetails(MemDetails prio1, MemDetails prio2);

/**
 * @brief return info->name
 * @param info 
 * @return char* 
 */
char* infoGetName(Info info);
/**
 * @brief set info->name as <name>
 * 
 * @param info 
 * @param name 
 * @return true on success
 * @return false on failure
 */
bool infoSetName(Info info, char* name);

/**
 * @brief return info->uid
 * @param info 
 * @return int 
 */
int infoGetUID(Info info);
/**
 * @brief set info->uid as <uid>
 * 
 * @param info 
 * @param uid 
 * @return true on success
 * @return false on failure
 */
bool infoSetUID(Info info, int uid);
/**
 * @brief return info->event_count
 * 
 * @param info 
 * @return unsigned int 
 */
unsigned int infoGetCount(Info info);
/**
 * @brief set info->event_count as <count>
 * 
 * @param info 
 * @param count 
 * @return true on success
 * @return false on failure
 */
bool infoSetCount(Info info, unsigned int count);

/**
 * @brief set all info's arguments
 * 
 * @param info 
 * @param name 
 * @param uid 
 * @param count 
 * @return true on success
 * @return false on failure
 */
bool infoSetAll(Info info, char* name, int uid, unsigned int count);

#endif //INFO_H
