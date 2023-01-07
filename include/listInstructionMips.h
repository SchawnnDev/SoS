#ifndef SOS_LISTINSTRUCTIONMIPS_H
#define SOS_LISTINSTRUCTIONMIPS_H

#include "utils.h"
#include "variable.h"

typedef struct goTo_t *GoTo;
struct goTo_t{
    int numberGoto;
    int numberTrue;
    int numberFalse;

    int unDefineGoto[CODE_TAB_MAX];
    int trueList[CODE_TAB_MAX];
    int falseList[CODE_TAB_MAX];

    GoTo previousGoTo;
};

typedef struct listGoTo_t *ListGoTo;
struct listGoTo_t{
    GoTo cursor;
    ListGoTo previousListGoTo;
};

typedef struct data_t *Data;
struct data_t{
    int numberData;
    char* lineData[DATA_TAB_MAX];
    Data previousData;
    Data nextData;
};

typedef struct code_t *Code;
struct code_t{
    int numberCode;
    int rangeCode;
    char *lineCode[CODE_TAB_MAX];
    Code previousCode;
    Code nextCode;
};

typedef struct {
    Data cursorData;
    Code cursorCode;
    ListGoTo cursorGoTo;
} listInstruction_t, *ListInstruction;

/*!
 * \fn Data initData( Data previousData)
 * \brief Fonction qui initialise la structure de data
 *
 * \param previousData : Data, la table précédante
 *
 * \return Data, un pointeur d'une structure de data
*/
Data initData(Data previousData);

/*!
 * \fn Code initCode( Code previousCode)
 * \brief Fonction qui initialise la structure de code
 *
 * \param previousCode : Data, la table précédante
 *
 * \return Code, un pointeur d'une structure de code
*/
Code initCode(Code previousCode);

/*!
 * \fn GoTo initGoTo(GoTo previousGoTo)
 * \brief Fonction qui initialise la structure de code
 *
 * \param previousGoTo : GoTo, la table précédante
 *
 * \return GoTo, un pointeur d'une structure de goto
*/
GoTo initGoTo(GoTo previousGoTo);

/*!
 * \fn ListInstruction initListInstruction()
 * \brief Fonction qui initialise la structure de liste d'instruction
 *
 * \return ListInstruction, un pointeur d'une structure de liste d'instruction
*/
ListInstruction initListInstruction();

/*!
 * \fn ListGoTo initListGoTo()
 * \brief Fonction qui initialise la structure de liste de GoTo
 *
 * \return ListGoTo, un pointeur d'une structure de liste de GoTo
*/
ListGoTo initListGoTo();

/*!
 * \fn void cleanData(Data addr)
 * \brief Fonction qui libère la mémoire de la structure data
 *
 * \param addr : ListInstruction, la structure text
*/
void cleanData(Data addr);

/*!
 * \fn void cleanCode(Code addr)
 * \brief Fonction qui libère la mémoire de la structure code
 *
 * \param addr : Code, la structure code
*/
void cleanCode(Code addr);

/*!
 * \fn void cleanGoTo(GoTo addr)
 * \brief Fonction qui libère la mémoire de la structure GoTo
 *
 * \param addr : GoTo, la structure code
*/
void cleanGoTo(GoTo addr);

/*!
 * \fn void cleanListInstruction(ListInstruction addr)
 * \brief Fonction qui libère la mémoire de la liste d'instruction
 *
 * \param addr : ListInstruction, la structure d'instruction
*/
void cleanListInstruction(ListInstruction addr);

/*!
 * \fn void cleanListGoTo(ListGoTo addr)
 * \brief Fonction qui libère la mémoire de la liste de goto
 *
 * \param addr : ListGoTo, la structure de goto
*/
void cleanListGoTo(ListGoTo addr);

/*!
 * \fn void addIntoData( ListInstruction addr)
 * \brief Fonction qui initialise une nouvelle structure de data
 *
 * \param addr : ListInstruction, la structure d'instruction
*/
void addStructData(ListInstruction addr);

/*!
 * \fn void addIntoData( ListInstruction addr, char* data)
 * \brief Fonction qui permet d'ajoute en fin de la structure de data
 *
 * \param addr : ListInstruction, la structure d'instruction
 * \param data : char*, le code mips à stocker
*/
void addIntoData(ListInstruction addr, char *data);

/*!
 * \fn void addStructCode( ListInstruction addr)
 * \brief Fonction qui initialise une nouvelle structure de code
 *
 * \param addr : ListInstruction, la structure d'instruction
*/
void addStructCode(ListInstruction addr);

/*!
 * \fn void addIntoCode( ListInstruction addr, char* code)
 * \brief Fonction qui permet d'ajoute en fin de la structure de code
 *
 * \param addr : ListInstruction, la structure d'instruction
 * \param code : char*, le code mips à stocker
*/
void addIntoCode(ListInstruction addr, char *code);

/*!
 * \fn int addIntoCodeWithIndex( Code addr, char* code, int index)
 * \brief Fonction qui permet d'ajoute en fin de la structure de code
 *
 * \param addr : Code, la structure de code
 * \param code : char*, le code mips à stocker
 * \param index : int, l'index dans le tableau de la structure de code
 *
 * \return int, un entier permettant de connaitre l'état de sortie du programme
*/
int addIntoCodeWithIndex( Code addr, char* code, int index);

/*!
 * \fn void addStructGoTo(ListInstruction addr)
 * \brief Fonction qui initialise une nouvelle structure de goto
 *
 * \param addr : ListInstruction, la structure d'instruction
*/
void addStructGoTo(ListInstruction addr);

/*!
 * \fn void addStructListGoTo(ListInstruction addr)
 * \brief Fonction qui initialise une nouvelle structure de goto
 *
 * \param addr : ListInstruction, la structure d'instruction
*/
void addStructListGoTo(ListInstruction addr);

/*!
 * \fn void addIntoCode( ListInstruction addr, char* code)
 * \brief Fonction qui permet d'ajoute un goto indéterminé
 *
 * \param addr : ListInstruction, la structure d'instruction
 * \param code : char*, le code que l'on souhaite écrire dans le code
*/
void addIntoUnDefineGoto(ListInstruction addr, char* code);

/*!
 * \fn void addIntoTrueList( ListInstruction addr, char* code)
 * \brief Fonction qui permet d'ajoute à la trueList
 *
 * \param addr : ListInstruction, la structure d'instruction
 * \param code : char*, le code que l'on souhaite écrire dans le code
*/
void addIntoTrueList(ListInstruction addr, char* code);

/*!
 * \fn void addIntoFalseList( ListInstruction addr, char* code)
 * \brief Fonction qui permet d'ajoute à la falseList
 *
 * \param addr : ListInstruction, la structure d'instruction
 * \param code : char*, le code que l'on souhaite écrire dans le code
*/
void addIntoFalseList(ListInstruction addr, char* code);

/*!
 * \fn void completeTrueList( ListInstruction addr, char* code )
 * \brief Fonction qui permet complete une ligne de la trueList
 *
 * \param addr : ListInstruction, la structure d'instruction
 * \param code : char*, la structure d'instruction
*/
int completeTrueList(ListInstruction addr, char *code);

/*!
 * \fn void completeFalseList( ListInstruction addr, char* code )
 * \brief Fonction qui permet complete une ligne de la falseList
 *
 * \param addr : ListInstruction, la structure d'instruction
 * \param code : char*, la structure d'instruction
*/
int completeFalseList(ListInstruction addr, char *code);

/*!
 * \fn void completeUnDefineGoto( ListInstruction addr, char* code )
 * \brief Fonction qui completer un goto indeterminé
 *
 * \param addr : ListInstruction, la structure d'instruction
 * \param code : char*, la structure d'instruction
*/
int completeUnDefineGoto( ListInstruction addr, char* code );

/*!
 * \fn void completeOneUnDefineGoto( ListInstruction addr, char* code )
 * \brief Fonction qui permet d'ajoute un goto indéterminé
 *
 * \param addr : ListInstruction, la structure d'instruction
 * \param code : char*, la structure d'instruction
*/
int completeOneUnDefineGoto(ListInstruction addr, char *code);

/*!
 * \fn int switchTrueFalseList(ListInstruction addr)
 * \brief Fonction qui switch la trueList et la falseList
 *
 * \param addr : ListInstruction, la structure d'instruction
*/
int switchTrueFalseList(ListInstruction addr);

/*!
 * \fn int writeToFile(ListInstruction list, FILE* file)
 * \brief Write all ListInstruction content into a file (line by line)
 *
 * \param list : ListInstruction, instructions structure
 * \param file : FILE*, File to write to
 * \return 0 if success, -1 if error
 */
int writeToFile(ListInstruction list, FILE *file);

/*!
 * \fn Move to first cursor in a Data structure
 * @param cursor Data
 * @return First cursor or NULL
 */
Data getFirstDataCursor(Data cursor);

/*!
 * \fn Move to first cursor in a Code structure
 * @param cursor Code
 * @return First cursor or NULL
 */
Code getFirstCodeCursor(Code cursor);

#endif //SOS_LISTINSTRUCTIONMIPS_H
