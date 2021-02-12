// region ################################################ 
/**
 * @file    adri_soft_logger.cpp
 *
 * @brief   fournit des fonctionnalités utiles pour déboguer les croquis via "printf" ou "LITTLEFS"
 * @see     
 * 
 * @author     AdriLighting   
 * @see        https://github.com/AdriLighting/adri_tools_v2
 * @date       06/02/2021 12:46:34
 *
 */

/*
    Utiliser la macro ADRI_LOGV ou ADRI_LOG

                region  lvl     trace   param   args
    ADRI_LOG(   parm_d, parm_c, parm_b, parm_a, ...)

    region  : attribuer un seemble de ADRI_LOG a la meme region, et de pouvoir gerer indivuduelement avec la classe "adriToolsLogger"
    lvl     : atribbuer un lvl associer a la position ou se trouve ADRI_LOG au sein de la fonction a debuger
    trace   : permet d'afficher la MACRO __LINE__ et ARDUINOTRACE_FUNCTION_NAME
    param   : "%s   - %d  - etcc"
    args    :  char - int - etcc
    

    ADRI_LOG permet d"afficher avec Serial ou bien d'ecrire dans un fichier text manager avec adriToolsLogger
     

    adriToolsLogger

    lvl
        0 start     t
        1 end       b
        2 random


    by mod 
        printer_displayMod_set

            9   only bt
            10  only t  

        printer_displayMod_set
            print et littfs  via printer_displayMod_pos


    printer_display_toggle
        activ/desactiv  print et littfs 
    spiff_toggle
        activ/desactiv  littfs

*/    
// endregion >>>> 


#include "adri_soft_logger.h"
#include <adri_tools_v2.h>

char* adriLogger_buffer1;

// region ################################################ color
#define ADRITOOLSLOGGER_COLOR_BLACK "30"
#define ADRITOOLSLOGGER_COLOR_RED "31"     // ERROR
#define ADRITOOLSLOGGER_COLOR_GREEN "32"   // INFO
#define ADRITOOLSLOGGER_COLOR_YELLOW "33"  // WARNING
#define ADRITOOLSLOGGER_COLOR_BLUE "34"
#define ADRITOOLSLOGGER_COLOR_MAGENTA "35"  // CONFIG
#define ADRITOOLSLOGGER_COLOR_CYAN "36"     // DEBUG
#define ADRITOOLSLOGGER_COLOR_GRAY "37"     // VERBOSE
#define ADRITOOLSLOGGER_COLOR_WHITE "38"
#define ADRITOOLSLOGGER_SECRET_BEGIN "\033[5m"
#define ADRITOOLSLOGGER_SECRET_END "\033[6m"

#define ADRITOOLSLOGGER_COLOR(COLOR) "\033[0;" COLOR "m"
#define ADRITOOLSLOGGER_BOLD(COLOR) "\033[1;" COLOR "m"
#define ADRITOOLSLOGGER_RESET_COLOR "\033[0m"
static const char *LOG_LEVEL_COLORS[] = {
    "",                                            // NONE
    ADRITOOLSLOGGER_BOLD(ADRITOOLSLOGGER_COLOR_RED),       // ERROR
    ADRITOOLSLOGGER_COLOR(ADRITOOLSLOGGER_COLOR_YELLOW),   // WARNING
    ADRITOOLSLOGGER_COLOR(ADRITOOLSLOGGER_COLOR_GREEN),    // INFO
    ADRITOOLSLOGGER_COLOR(ADRITOOLSLOGGER_COLOR_MAGENTA),  // CONFIG
    ADRITOOLSLOGGER_COLOR(ADRITOOLSLOGGER_COLOR_CYAN),     // DEBUG
    ADRITOOLSLOGGER_COLOR(ADRITOOLSLOGGER_COLOR_GRAY),     // VERBOSE
    ADRITOOLSLOGGER_COLOR(ADRITOOLSLOGGER_COLOR_WHITE),    // VERY_VERBOSE
};    
// endregion >>>> color


//################# adriToolsLogger
//################################################

// region ################################################ consctructor
adriToolsLogger * adriToolsLogger_ptr = nullptr;
/**
 * @brief      ptr ver la dernierre instance de la classe "adriToolsLogger"
 *
 * @author     AdriLighting   
 * @see        https://github.com/AdriLighting/adri_tools_v2
 * @date       06/02/2021 12:46:34
 *
 * @return     ptr
 */
adriToolsLogger * adriToolsLogger_ptrGet(){
    if (adriToolsLogger_ptr == nullptr) adriToolsLogger_ptr = new adriToolsLogger();
    return adriToolsLogger_ptr;
}
adriToolsLogger::adriToolsLogger(){
    adriToolsLogger_ptr = this;
    adriLogger_buffer1 = new char[255];
    ADRI_LOG(-1, 2, 2, "", "");
}   
adriToolsLogger::adriToolsLogger(int size){
    adriToolsLogger_ptr = this;
    adriLogger_buffer1 = new char[size];
    ADRI_LOGV(-1, 2, 2, size, "", "");
    ADRI_LOG(-1, 2, 2, "", "");
}   
void adriToolsLogger::setup(){
    spiff_setup();
}   
// endregion >>>> consctructor
// region ################################################ tools
/**
 * @brief      création du tempstamp
 *
 * @author     AdriLighting   
 * @see        https://github.com/AdriLighting/adri_tools_v2
 * @date       06/02/2021 12:46:34
 *
 * @return     string
 */
String adriToolsLogger::timeStamp()
{
  char result[20];
  snprintf(result, 20, "%lu", millis());
  return String(result);
}
/**
 * @brief      recherche des similitude dans une chaine de caractére
 *
 * @author     AdriLighting   
 * @see        https://github.com/AdriLighting/adri_tools_v2
 * @date       06/02/2021 12:46:34
 *
 * @param[in]  str    le caractére a chercher
 * @param[in]  sfind  la chaine a analyser
 *
 * @return     le nombre de similitude
 */
int adriToolsLogger::strContains(char str, const char *sfind) {
    int result = 0;
    int len = strlen(sfind);
    for (int i = 0; i < len; ++i) {   
        if ( sfind[i] == str ) result++;

    }   
    return result; 
}    
// endregion >>>> tools

// region ################################################ display mod && lvl

/**
 * @brief       active  le "printf" et "littfs" uniquement pour le level voullu pour toutes les region
 *
 * @author     AdriLighting   
 * @see        https://github.com/AdriLighting/adri_tools_v2
 * @date       06/02/2021 12:46:34
 *
 * @param[in]   lvl   level voullu
 */
void adriToolsLogger::printer_displayMod_set(int lvl){
    printer_displayMod_pos = lvl;
    ADRI_LOGV(-1, 2, 2, printer_displayMod_pos, "", "");  
}    
// endregion >>>> display mod

// region ################################################ toggle
/**
 * @brief      acitve/desactive le timestamp
 * 
 * @author     AdriLighting   
 * @see        https://github.com/AdriLighting/adri_tools_v2
 * @date       06/02/2021 12:46:34
 *
 */
void adriToolsLogger::printer_timestamp_toggle(){
    printer_timestamp = !printer_timestamp;
    ADRI_LOGV(-1, 2, 2, printer_timestamp, "", "");  
}
/**
 * @brief      activer/desactive le freeheap
 * 
 * @author     AdriLighting   
 * @see        https://github.com/AdriLighting/adri_tools_v2
 * @date       06/02/2021 12:46:34
 *
 */
void adriToolsLogger::printer_heap_toggle(){
    printer_heap = !printer_heap;
    ADRI_LOGV(-1, 2, 2, printer_heap, "", "");  
}
/**
 * @brief      active/desactive le "printf" et "littfs" pour toutes les region
 * 
 * @author     AdriLighting   
 * @see        https://github.com/AdriLighting/adri_tools_v2
 * @date       06/02/2021 12:46:34
 *
 */
void adriToolsLogger::printer_display_toggle(){
    printer_display = !printer_display;
    ADRI_LOGV(-1, 2, 2, printer_display, "", "");  
}
/**
 * @brief      active/desactive le "littfs" pour toutes les region
 * 
 * @author     AdriLighting   
 * @see        https://github.com/AdriLighting/adri_tools_v2
 * @date       06/02/2021 12:46:34
 *
 */
void adriToolsLogger::spiff_toggle(){
    spiff_sav = !spiff_sav;
    ADRI_LOGV(-1, 2, 2, spiff_sav, "", "");     
}
// endregion >>>> toggle

// region ################################################ SPIFF
/**
 * @brief      initialize le fichier log
 * @details    utiliser pour chaque demmarage de l'esp, inscript dans le fichier log la raisson du dernier reset de l'esp 
 * 
 * @author     AdriLighting   
 * @see        https://github.com/AdriLighting/adri_tools_v2
 * @date       06/02/2021 12:46:34
 *
 */
void adriToolsLogger::spiff_setup(){


    ADRI_LOG(-1, 0, 2,"","");

    // Dir dir = LittleFS.openDir(spiff_folder);

    // int totalFile = 0;
    // while (dir.next()) {
    //     if (dir.isDirectory()) {

    //     } else  {
    //      totalFile++;
    //     }
    // }

    // spiff_fileName = String(totalFile+1) + ".txt";

    spiff_fileName = "log.txt";

    spiff_addLine("\n");
    adri_toolsV2 * _tools = adri_toolsv2Ptr_get();
    _tools->ESP_boot_info();
    int size = _tools->tempStrArraySize;
    for (int i = 0; i < size; ++i)
    {
        spiff_addLine(_tools->tempStrArray[i]);
    }

    ADRI_LOG(-1, 1, 2,"","");
}   
/**
 * @brief      stock le fichier log dans une string
 *
 * @author     AdriLighting   
 * @see        https://github.com/AdriLighting/adri_tools_v2
 * @date       06/02/2021 12:46:34
 *
 * @param[int/out]  result  totalitée du fichier log
 */
void adriToolsLogger::spiff_get(String & ret){
    // ADRI_LOG(-1, 0, 2,"","");

    String lines = "";

    File file = LittleFS.open(spiff_folder+"/"+spiff_fileName, "r");

    if (file) {
        while (file.position()<file.size()) {
            String xml = file.readStringUntil('\n');
            if (xml != "") {
                lines += xml + "\n";
            }
        }
        file.close(); 
    }

    ret = lines;

    // ADRI_LOG(-1, 1, 2,"","");
}  
/**
 * @brief      ajoute un ligne au fichier log
 *
 * @author     AdriLighting   
 * @see        https://github.com/AdriLighting/adri_tools_v2
 * @date       06/02/2021 12:46:34
 *
 * @param[in]  result  la ligne a ajouter
 */
void adriToolsLogger::spiff_addLine(String result){
    // String old;
    // spiff_get(old);
    File file = LittleFS.open(spiff_folder+"/"+spiff_fileName, "a");
    file.println(result);
    file.close();

}
/**
 * @brief      printf du fichier log
 * 
 * @author     AdriLighting   
 * @see        https://github.com/AdriLighting/adri_tools_v2
 * @date       06/02/2021 12:46:34
 *
 */
void adriToolsLogger::spiff_readCurrent(){
    ADRI_LOG(-1, 0, 2, "", "");

    fsprintf("\n[spiff_readCurrent]\n"); 
    char buffer[512];
    String print = "";
    int nbr = 0;
    File file = LittleFS.open(spiff_folder+"/"+spiff_fileName, "r");
    if (file) {
        while (file.position()<file.size()) {
            String xml = file.readStringUntil('\n');
            if (xml != "") {
                // Serial.println(xml);
                sprintf(buffer, "[%5d] %s", nbr, xml.c_str());
                print += String(buffer) + "\n";
                fsprintf("%s\n", buffer);
                delay(5);
                nbr++;
            }
        }
        file.close(); 
    }
    ADRI_LOG(-1, 1, 2, "", "");
}
/**
 * @brief      supprime le fichier log de la mem spiff
 * 
 * @author     AdriLighting   
 * @see        https://github.com/AdriLighting/adri_tools_v2
 * @date       06/02/2021 12:46:34
 *
 */
void adriToolsLogger::spiff_removeCurrent(){
    LittleFS.remove(spiff_folder+"/"+spiff_fileName);
}
    
// endregion >>>> SPIFF

// region ################################################ minu
/**
 * @brief      fonction pour le keyboard, commnds menu
 *
 * @author     AdriLighting   
 * @see        https://github.com/AdriLighting/adri_tools_v2
 * @date       06/02/2021 12:46:34
 *
 * @param[in]  cmd    The command
 * @param[in]  value  The value
 *
 * @return     ""
 */
String adriToolLogger_serialMenu_cmd(String cmd, String value){
    adriToolsLogger * _looger = adriToolsLogger_ptrGet();
    ADRI_LOG(-1, -1, 0, "<<<", "");
    ADRI_LOG(-1, 0, 2,"","");
    ADRI_LOG(-1, 2, 2, "cmd: %s - value: %S", cmd.c_str(), value.c_str());  
    int pos = value.toInt();
    if (value == "a") {
        _looger->serial_menu();
    } else if (value == "z") {
        _looger->printer_display_toggle();
    } else if (value == "e") {
        _looger->printer_heap_toggle();
    } else if (value == "r") {
        _looger->printer_timestamp_toggle();
    } else if (value == "t") {
        _looger->spiff_toggle();
    } else if (value == "y") {
        _looger->spiff_readCurrent();
    } else if (value == "u") {
        _looger->spiff_removeCurrent();
    } else {
        _looger->printer_displayMod_set(pos);  
    }
    ADRI_LOG(-1, 1, 2,"","");
    ADRI_LOG(-1, -1, 0, ">>>", "");
    return "";
}
/**
 * @brief      printf des comandes disponnible
 * 
 * @author     AdriLighting   
 * @see        https://github.com/AdriLighting/adri_tools_v2
 * @date       06/02/2021 12:46:34
 *
 */
void adriToolsLogger::serial_menu() {
    adri_toolsV2 * _ptr = adri_toolsv2Ptr_get();
    if (_ptr->tempStrArray != nullptr) delete[] _ptr->tempStrArray;
    int size = 11;
    _ptr->tempStr_sizeSet(size);
    _ptr->tempStrArray      = new String[size];

    _ptr->tempStrArray[0]   = "\nPrinter lvl\n";
    _ptr->tempStrArray[1]  = _ptr->info_parm(F("9"),   F("only bt"), 10);
    _ptr->tempStrArray[2]  = _ptr->info_parm(F("10"),  F("only t"), 10);
    _ptr->tempStrArray[3]  = "\nMenu\n";
    _ptr->tempStrArray[4]  = _ptr->info_parm(F("a"),   F("menu"), 10);
    _ptr->tempStrArray[5]  = _ptr->info_parm(F("z"),   F("printer_display_toggle"), 10);
    _ptr->tempStrArray[6]  = _ptr->info_parm(F("e"),   F("printer_heap_toggle"), 10);
    _ptr->tempStrArray[7]  = _ptr->info_parm(F("r"),   F("printer_timestamp_toggle"), 10);
    _ptr->tempStrArray[8]  = _ptr->info_parm(F("t"),   F("spiff activate"), 10);
    _ptr->tempStrArray[9]  = _ptr->info_parm(F("y"),   F("spiff read"), 10);
    _ptr->tempStrArray[10]  = _ptr->info_parm(F("u"),   F("spiff remove"), 10);    
    _ptr->tempStr_print();        
}     
// endregion >>>> minu

// region ################################################ print
/**
 * @brief      fonction pour determiner si un "printf" ou "littlefs" est possible
 *
 * @author     AdriLighting   
 * @see        https://github.com/AdriLighting/adri_tools_v2
 * @date       06/02/2021 12:46:34
 *
 * @param[in]  region  The region
 * @param[in]  lvl     The level
 *
 * @return     true si un "printf" ou "littlefs" est possible
 */
boolean adriToolsLogger::printer_displayMod(int region, int lvl){
    if (!printer_display) return false;

    if (region == -2) return false; 

    if (printer_displayMod_pos == 9) {
        if (lvl < 0) return true;  
        if ((lvl != 0) && (lvl != 1)) return false;
    }
    if (printer_displayMod_pos == 10) {
        if (lvl < 0) return true;  
        if (lvl!=0) return false;
    } 

    return true;
}   

/**
 * @brief      fonction final pour le "printf" et "littfs"
 * @details    ajouter timestamp et freeheap
 * @details    check si la region est activer
 *
 * @author     AdriLighting   
 * @see        https://github.com/AdriLighting/adri_tools_v2
 * @date       06/02/2021 12:46:34
 *
 * @param[in]  addLine  check if can print with "littfls"
 * @param[in]  region   The region
 * @param      buffer   The buffer
 */
void adriToolsLogger::printer_serial(boolean addLine, int region, int lvl, String func, char * buffer){
    String          result  = String(buffer);
    unsigned int    len     = result.length();
    char            bufferPrint[len + 255];
    int             heap    = ESP.getFreeHeap();
    String          cTtime;
    // adri_toolsv2Ptr_get()->ntpTime_getTime(cTtime);
    cTtime = adri_toolsv2Ptr_get()->om_time();
    const char      * color  = LOG_LEVEL_COLORS[0];
    String title = "";

    if ((printer_timestamp) && (printer_heap)) {
        fssprintf(bufferPrint, "[%-15s][%-8d]%s", cTtime.c_str(), heap, buffer);
    }
    else {
        if (printer_timestamp)  fssprintf(bufferPrint, "[%-15s]%s", cTtime.c_str(), buffer);
        if (printer_heap)       fssprintf(bufferPrint, "[%-8d]%s", heap, buffer);
    } 

    if (spiff_sav && addLine) {
        spiff_addLine(title+String(bufferPrint));
    }

    String fBuffer = String(bufferPrint);
    Serial.println(title+fBuffer);

    if (adri_toolsv2Ptr_get() != nullptr) {
        if (adri_toolsv2Ptr_get()->_telnetPut != NULL) {
            int pos = 0;
            switch (lvl) {
                case -1:
                    pos = 7; 
                break; 
                case -2:
                    pos = 6; 
                break;                                            
                case 0:
                    pos = 2;
                break;
                case 1:
                    pos = 2;
                break;
                case 2:
                    pos = 3;
                break;                    
                default:
                    pos = 7;
                break;
            }
            color = LOG_LEVEL_COLORS[pos];
            fssprintf(bufferPrint, "%s%s\n", color, fBuffer.c_str());
            adri_toolsv2Ptr_get()->_telnetPut(String(bufferPrint));
        }
    }
}    
// endregion >>>> print

// region ################################################ printer line
void esp_log_print_lines(boolean addLine, int region, int lvl, int mod, String pLine, String func, String var, const char *tag){
    adriToolsLogger * _looger = adriToolsLogger_ptrGet();
    String      pTag        = String(tag);
    int         searchCR    = 0;
                searchCR    = _looger->strContains('\n', tag);    
    String      varP        = var;
    String      title       = "";


    switch (lvl) {
        case 0:
            title = "[<<START>>]";
        break;
        case 1:
            title = "[<<END>>]";    
        break;
        default:break;
    }    

    if (pLine != "") {
        if (pTag != "")     {
            String cr = "";
            int len = strlen(tag);
            if (searchCR > 0)   {
                if ((pTag.substring(len-1) == "\n") && (pTag.substring(0, 1) == "\n")  ) {
                    pTag.remove(len-1, 1);
                    pTag.remove(0, 1);
                    cr = "\n";
                } else {
                    if (pTag.substring(0,1)     == "\n") {pTag.remove(0, 1);}
                    if (pTag.substring(len-1)   == "\n") {pTag.remove(len-1, 1);}
                    cr = "\n";
                }
                if (varP != "") {
                    fssprintf(adriLogger_buffer1, "[%-97s][%-20s]%s%s\t[%s]", 
                            pLine.c_str(), 
                            varP.c_str(),
                            title.c_str(), 
                            cr.c_str(),
                            pTag.c_str()
                        );
                } else {
                    fssprintf(adriLogger_buffer1, "[%-97s]%s%s\t[%s]", 
                            pLine.c_str(), 
                            title.c_str(), 
                            cr.c_str(),
                            pTag.c_str()
                        );                    
                } 
            }  else {
                if (varP != "") {
                    fssprintf(adriLogger_buffer1, "[%-97s][%-20s]%s[%s]", 
                            pLine.c_str(), 
                            varP.c_str(),
                            title.c_str(), 
                            pTag.c_str()
                        ); 
                } else {
                    fssprintf(adriLogger_buffer1, "[%-97s]%s[%s]", 
                            pLine.c_str(), 
                            title.c_str(), 
                            pTag.c_str()
                        ); 
                }
            }
        } else {
            if (varP != "") {fssprintf(adriLogger_buffer1, "[%-97s][%-20s]%s", pLine.c_str(), varP.c_str(), title.c_str()); }
            else {fssprintf(adriLogger_buffer1, "[%-97s]%s", pLine.c_str(), title.c_str()); }
        }       
        
    }
    else {

        if (pTag != "")     {
            String cr = "";
            int len = strlen(tag);
            if (searchCR > 0)   {
                if ((pTag.substring(len-1) == "\n") && (pTag.substring(0, 1) == "\n")  ) {
                    pTag.remove(len-1, 1);
                    pTag.remove(0, 1);
                    cr = "\n";
                } else {
                    if (pTag.substring(0,1)     == "\n") {pTag.remove(0, 1);}
                    if (pTag.substring(len-1)   == "\n") {pTag.remove(len-1, 1);}
                    cr = "\n";
                }
                if (varP != "") {
                    fssprintf(adriLogger_buffer1, "[%-20s]%s%s\t[%s]", 
                            varP.c_str(),
                            title.c_str(), 
                            cr.c_str(),
                            pTag.c_str()
                        );
                } else {
                    fssprintf(adriLogger_buffer1, "%s%s\t[%s]", 
                            title.c_str(), 
                            cr.c_str(),
                            pTag.c_str()
                        );                    
                } 
            }  else {
                if (varP != "") {
                    fssprintf(adriLogger_buffer1, "[%-20s]%s[%s]", 
                            varP.c_str(),
                            title.c_str(), 
                            pTag.c_str()
                        ); 
                } else {
                    fssprintf(adriLogger_buffer1, "%s[%s]", 
                            title.c_str(), 
                            pTag.c_str()
                        ); 
                }
            }
        } else {
            if (varP != "") {fssprintf(adriLogger_buffer1, "[%-20s]%s", varP.c_str(), title.c_str()); }
            else {fssprintf(adriLogger_buffer1, "%s",  title.c_str()); }
        } 
    }

    _looger->printer_serial(addLine, region, lvl, func, adriLogger_buffer1 );
}
// endregion >>>> printer line

// region ################################################ esp_log_printf_
void  esp_log_printf_(int region, int lvl, int mod, String line, String func, String var, const char *tag) {
    if (!adriToolsLogger_ptrGet()->printer_displayMod(region, lvl)) return;
    String pLine = "";
    switch (mod) {
        case 1:
            pLine = line;
            break;
        case 2:
            pLine = line + " " + func;       
            break;
        default:
            break;
    }
    esp_log_print_lines(true, region, lvl, mod, pLine, func, var, tag );
}
void  esp_log_printf_(int region, int lvl, int mod, String line, String func, const char *tag) {
    if (!adriToolsLogger_ptrGet()->printer_displayMod(region, lvl)) return;
    String pLine = "";
    switch (mod) {
        case 1:
            pLine = line;
            break;
        case 2:
            pLine = line + " " + func;       
            break;
        default:
            break;
    }
    esp_log_print_lines(true, region, lvl, mod, pLine, func, (char*)"", tag );
} 
void  esp_log_printf_(boolean addLine, int region, int lvl, int mod, String line, String func, const char *tag) {
    if (!adriToolsLogger_ptrGet()->printer_displayMod(region, lvl)) return;
    String pLine = "";
    switch (mod) {
        case 1:
            pLine = line;
            break;
        case 2:
            pLine = line + " " + func;       
            break;
        default:
            break;
    }
    esp_log_print_lines(addLine, region, lvl, mod, pLine, func, (char*)"", tag );
}     
// endregion >>>> 