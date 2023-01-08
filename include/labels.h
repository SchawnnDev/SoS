#ifndef SOS_LABELS_H
#define SOS_LABELS_H

#define LABEL_NAME "_lbl_%d"
#define LABEL_NAME_LEN 6
#define LABEL_FOR "_for_%d"

/**
 * Creates a new unique label in data
 * @return const char*, created label
 */
const char* createNewLabel();

/**
 * Creates a new unique for label in data
 * @return const char*, created label
 */
const char* createNewForLabel();

/**
 * Creates a new unique for label in data
 * @return const char*, created unique for label
 */
const char* getForLabel();

#endif //SOS_LABELS_H
