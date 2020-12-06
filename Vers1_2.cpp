#include <Arduino.h>
#include <OneWire.h>
#include <SPI.h>
#include <DallasTemperature.h>
#include <PubSubClient.h>
#include <TimeLib.h>
#include <BlynkSimpleEsp32.h>
#include <Ethernet.h>
#include <WidgetRTC.h>
#include <LiquidCrystal_I2C.h>
#include <CAN.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "FritziVonHams";
char pass[] = "78313171786307415199";

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
//char auth[] = "UwNCHGrGQzdb3LE7xRCPiy3VK7PqUfBf";
  char auth[] = "UwNCHGrGQzdb3LE7xRCPiy3VK7PqUfBf";

//LiquidCrystal_I2C LCD(0x27,20,4);

boolean BewaesserungsStart                    = HIGH;
boolean ManualOperation_Active                = LOW;
boolean AutoOperation_Prepare                 = LOW;
boolean PFG_Merker_AutoOperation_Active       = LOW;
boolean NFG_Merker_AutoOperation_Active       = HIGH;
boolean PFL_AutoOperation_Active              = LOW;
boolean NFL_AutoOperation_Active              = LOW;
boolean Freigabe_Waesserungszeit_Lesen        = LOW;
boolean AutoOperation_Active                  = LOW;
boolean Beet_1_PrepareStart                   = LOW;
boolean Beet_1_Manu_PrepareStart              = LOW;
boolean PFL_BeeteActive                       = LOW; //Flanke wenn eine der Beetebewässerung eingeschaltet wird
boolean Beet_1_WaessernActive                 = LOW;
boolean Beet_1_Gewaessert                     = LOW;
boolean Beet_2_PrepareStart                   = LOW;
boolean Beet_2_Manu_PrepareStart              = LOW;
boolean Beet_2_WaessernActive                 = LOW;
boolean Beet_2_Gewaessert                     = LOW;
boolean Beet_3_PrepareStart                   = LOW;
boolean Beet_3_Manu_PrepareStart              = LOW;
boolean Beet_3_WaessernActive                 = LOW;
boolean Beet_3_Gewaessert                     = LOW;
boolean Beet_4_PrepareStart                   = LOW;
boolean Beet_4_Manu_PrepareStart              = LOW;
boolean Beet_4_WaessernActive                 = LOW;
boolean Beet_4_Gewaessert                     = LOW;
boolean PFL_RasenActive                       = LOW; //Flanke wenn eine der Rasenbewässerung eingeschaltet wird
boolean Rasen_1_PrepareStart                  = LOW;
boolean Rasen_1_Manu_PrepareStart             = LOW;
boolean Rasen_1_WaessernActive                = LOW;
boolean Rasen_1_Gewaessert                    = LOW;
boolean Rasen_2_PrepareStart                  = LOW;
boolean Rasen_2_Manu_PrepareStart             = LOW;
boolean Rasen_2_WaessernActive                = LOW;
boolean Rasen_2_Gewaessert                    = LOW;
boolean PFL_TopfpflanzenActive                = LOW; //Flanke wenn eine der Topfpflanzenbewässerung eingeschaltet wird
boolean TopfPflanzen_1_PrepareStart           = LOW;
boolean TopfPflanzen_1_Manu_PrepareStart      = LOW;
boolean TopfPflanzen_1_WaessernActive         = LOW;
boolean TopfPflanzen_1_Gewaessert             = LOW;
boolean TopfPflanzen_2_PrepareStart           = LOW;
boolean TopfPflanzen_2_Manu_PrepareStart      = LOW;
boolean TopfPflanzen_2_WaessernActive         = LOW;
boolean TopfPflanzen_2_Gewaessert             = LOW;
boolean Wasserstelle_EinActive                = LOW;
boolean Bewaesserungspumpe                    = LOW;
boolean AnlageEntwaessern_Active              = LOW;
boolean Trafo_24VAC                           = LOW;
boolean Verriegelung_Waessern                 = LOW;
boolean AutoBewaesserungsZeitAktiv            = LOW;
boolean Monday_Choose                         = LOW;
boolean Tuesday_Choose                        = LOW;
boolean Wednesday_Choose                      = LOW;
boolean Thursday_Choose                       = LOW;
boolean Friday_Choose                         = LOW;
boolean Saturday_Choose                       = LOW;
boolean Sunday_Choose                         = LOW;
boolean Current_WeekdayIsChoose               = LOW;

int Manu_Function                     = 0;
int Auto_Function                     = 0;
int Beet_1_Choose                     = 0;
int Beet_2_Choose                     = 0;
int Beet_3_Choose                     = 0;
int Beet_4_Choose                     = 0;
int Rasen_1_Choose                    = 0;
int Rasen_2_Choose                    = 0;
int TopfPflanzen_1_Choose             = 0;
int TopfPflanzen_2_Choose             = 0;
int Wasserstelle_Choose               = 0;
int Entwaessern_Choose                = 0; // Entwässern der gesamten Anlage (alle Ventile offen Pumpe aus)
int SetTime_Beete                     = 0;
int SetTime_Rasen                     = 0;
int SetTime_Topfpflanzen              = 0;
int SetTime_Wasserstellen             = 0;
int Bewaesserungszeit_Beete           = 0;
int Bewaesserungszeit_Rasen           = 0;
int Bewaesserungszeit_Topfpflanzen    = 0;
int Bewaesserungszeit_Wasserstellen   = 0;
int Restzeit_Beete                    = 0;
int Restzeit_Rasen                    = 0;
int Restzeit_Topfpflanzen             = 0;
int Restzeit_Wasserstellen            = 0;
int SThour;
int STmin;
int STsec;
int SPhour;
int SPmin;
int SPsec;
int Current_day                       = 0;
int Current_weekday                   = 0;
int Current_hour                      = 0;
int Current_min                       = 0;


// Mapping of analog pins as digital I/O
static const uint8_t GPIO_2   = 2;  //Beet_1
static const uint8_t GPIO_4   = 4;  //Beet_2
static const uint8_t GPIO_5   = 5;  //Beet_3
static const uint8_t GPIO_12  = 12;
static const uint8_t GPIO_13  = 13;
static const uint8_t GPIO_14  = 14;
static const uint8_t GPIO_15  = 15;
static const uint8_t GPIO_16  = 16; //Topfpflanzen_2
static const uint8_t GPIO_17  = 17; //Topfpflanzen_1
static const uint8_t GPIO_18  = 18; //Beet_4   
static const uint8_t GPIO_19  = 19; //Rasen_1
static const uint8_t GPIO_21  = 21; //Rasen_2
static const uint8_t GPIO_22  = 22; //Manuelle Wasserstelle Ein- Aus
static const uint8_t GPIO_23  = 23; //Pumpe Ein- Aus => Wird mit Anforderung einer Bewässerung eingeschaltet
static const uint8_t GPIO_25  = 25; //Trafo Ein- Aus => Wird mit auto- oder manuellen Betrieb eingeschaltet
static const uint8_t GPIO_26  = 26; //Reserve

String Mail_str;
String startTime;
String stopTime;
String currentDate;
String currentTime;

String String_Bewaesserungszeit_Beet1;
String String_Bewaesserungszeit_Beet2;
String String_Bewaesserungszeit_Beet3;
String String_Bewaesserungszeit_Beet4;
String String_Bewaesserungszeit_Rasen1;
String String_Bewaesserungszeit_Rasen2;
String String_Bewaesserungszeit_Topfpflanzen1;
String String_Bewaesserungszeit_Topfpflanzen2;

#define Auto_OnOff V0
#define Manu_OnOff V10
#define Beet_1_LED V1
#define Beet_2_LED V2
#define Beet_3_LED V3
#define Beet_4_LED V4
#define Beet_1_OnOff V11
#define Beet_2_OnOff V12
#define Beet_3_OnOff V13
#define Beet_4_OnOff V14
#define Rasen_1_OnOff V15
#define Rasen_2_OnOff V16
#define Slide_SetTime V11
#define BLYNK_MAX_SENDBYTES 1200
#define CR "\x0D"


BlynkTimer timer;
WidgetRTC rtc;
WidgetLCD lcd(V110);

/***************************************************************************************************************************************************************************************
 ***************************************************************************************************************************************************************************************
 Digital clock display of the time
 Dieser Programmabschnitt wird alle 5000 ms einmal bearbeitet
 In diesem Abschnitt wird wird die Bewässerungszeit überwacht und angezeigt
 Ist die Bewässerungszeit abgelaufen wird der aktive Status zurückgesetzt damit ein neuer Bewässerungszyklus gesetzt werden kann
 ***************************************************************************************************************************************************************************************
 ***************************************************************************************************************************************************************************************/
void clockDisplay()
{    
  // Beim Start der Bewässerung Restzeit setzen
    if (BewaesserungsStart == HIGH)
    {
      BewaesserungsStart = LOW;
      //Eingegebene Bewässerungszeit von [min]  in [s] umrechnen
      Restzeit_Beete = (SetTime_Beete * 60); 
      Restzeit_Rasen = (SetTime_Rasen * 60);
      Restzeit_Topfpflanzen = (SetTime_Topfpflanzen * 60);
      Restzeit_Wasserstellen = (SetTime_Wasserstellen * 60);
    }
    /*****************************************************************************************************************************************************************
     * Reststellen der Bewässerungszonen bestellen
     *****************************************************************************************************************************************************************/
    //***********************************************************************************************************
    // Wenn Beete aktiv => Restzeit der Beete berechnen
    //***********************************************************************************************************
    if ((Beet_1_WaessernActive == HIGH || Beet_2_WaessernActive == HIGH || Beet_3_WaessernActive == HIGH || Beet_4_WaessernActive == HIGH) && (Restzeit_Beete > 0))
        {
            if (Restzeit_Beete < 5) 
            {
            Restzeit_Beete = 0;   
            }
            else
            {     
            Restzeit_Beete = (Restzeit_Beete - 5);   
            }
        }

      //***********************************************************************************************************
      // Wenn Rasen aktiv => Restzeit der Rasenbewässerung berechnen
      //***********************************************************************************************************
    if ((Rasen_1_WaessernActive == HIGH || Rasen_2_WaessernActive == HIGH) && (Restzeit_Rasen > 0))
        {
            if (Restzeit_Rasen < 5)
            {
            Restzeit_Rasen = 0;  
            }
            else
            {
            Restzeit_Rasen = (Restzeit_Rasen - 5);   
            }
        }
  
      //***********************************************************************************************************
      // Wenn Topfpflanzen aktiv => Restzeit der Topfpflanzenbewässerung berechnen
      //***********************************************************************************************************

    if ((TopfPflanzen_1_WaessernActive == HIGH ||TopfPflanzen_2_WaessernActive == HIGH) && (Restzeit_Topfpflanzen > 0))
        {
            if (Restzeit_Topfpflanzen < 5)
            {
            Restzeit_Topfpflanzen = 0;     
            }
            else
            {
            Restzeit_Topfpflanzen = (Restzeit_Topfpflanzen - 5); 
            }
        }

      /************************************************************************************************************
            Wenn Wasserstelle aktiv => Restzeit der Wasserstellen berechnen
      *************************************************************************************************************/
    if ((Wasserstelle_EinActive == HIGH) && (Restzeit_Wasserstellen > 0))
        {
        if (Restzeit_Wasserstellen < 5) // Restlaufzeit der Wassserstelle < 5s 
            {
            Restzeit_Wasserstellen = 0;
            }
        else
            {
             Restzeit_Wasserstellen = (Restzeit_Wasserstellen - 5); // mit jedem Durchlauf Restzeit um 5s verringern
            }
        }

    /*********************************************************************************************************************************************************************************************
     *********************************************************** Autobetrieb einschalten *********************************************************************************************************
        Wenn der Automatikmodus aktiv ist (angewählt und Autozeit aktiv) und keine Bewässerung aktiv ist überprüfen ob eine Bewässerungstelle vorbereitet eingeschaltet ist 
        diese wird dann aktiviert
     *********************************************************************************************************************************************************************************************
     *********************************************************************************************************************************************************************************************/  
    if (((AutoOperation_Active == HIGH) && 
        Beet_1_WaessernActive == LOW && Beet_2_WaessernActive == LOW && Beet_3_WaessernActive == LOW && Beet_4_WaessernActive == LOW && 
        Rasen_1_WaessernActive == LOW && Rasen_2_WaessernActive == LOW && TopfPflanzen_1_WaessernActive == LOW && TopfPflanzen_2_WaessernActive == LOW) &&
        (Beet_1_PrepareStart == HIGH || Beet_2_PrepareStart == HIGH || Beet_3_PrepareStart == HIGH || Beet_4_PrepareStart == HIGH || Rasen_1_PrepareStart == HIGH || 
        Rasen_2_PrepareStart == HIGH || TopfPflanzen_1_PrepareStart == HIGH || TopfPflanzen_2_PrepareStart == HIGH))
       {   
        if (Beet_1_PrepareStart == HIGH && Verriegelung_Waessern == LOW) 
            {
            Beet_1_WaessernActive = HIGH;
            Beet_1_PrepareStart = LOW;
            Restzeit_Beete = (SetTime_Beete * 60);
            Blynk.virtualWrite(V1, 255); //LED Beet 1 ein
            Blynk.virtualWrite(V11, HIGH); //Taster Beet1 auf eingeschaltet stellen 
            }
           
        else if (Beet_2_PrepareStart == HIGH && Verriegelung_Waessern == LOW)     
            {
            Beet_2_WaessernActive = HIGH;
            Beet_2_PrepareStart = LOW;
            Verriegelung_Waessern = HIGH;
            Restzeit_Beete = (SetTime_Beete * 60);
            Blynk.virtualWrite(V2, 255); //LED Beet2 ein
            Blynk.virtualWrite(V12, HIGH); //Taster Beet2 auf eingeschaltet stellen
            }
            
        else if (Beet_3_PrepareStart == HIGH && Verriegelung_Waessern == LOW) 
            {
            Beet_3_WaessernActive = HIGH;
            Beet_3_PrepareStart = LOW;
            Verriegelung_Waessern = HIGH;
            Restzeit_Beete = (SetTime_Beete * 60);
            Blynk.virtualWrite(V3, 255); //LED Beet3 ein
            Blynk.virtualWrite(V13, HIGH); //Taster Beet3 auf eingeschaltet stellen
            }
           
          else if (Beet_4_PrepareStart == HIGH && Verriegelung_Waessern == LOW) 
            {
            Beet_4_WaessernActive = HIGH;
            Beet_4_PrepareStart = LOW;
            Verriegelung_Waessern = HIGH;
            Restzeit_Beete = (SetTime_Beete * 60);
            Blynk.virtualWrite(V4, 255); //LED Beet4 ein
            Blynk.virtualWrite(V14, HIGH); //Taster Beet4 auf eingeschaltet stellen
            }

          else if (Rasen_1_PrepareStart == HIGH && Verriegelung_Waessern == LOW) 
            {
            Rasen_1_WaessernActive = HIGH;
            Rasen_1_PrepareStart = LOW;
            Verriegelung_Waessern = HIGH;
            Restzeit_Rasen = (SetTime_Rasen * 60);
            Blynk.virtualWrite(V5, 255); //LED Rasen1 ein
            Blynk.virtualWrite(V15, HIGH); //Taster Rasen1 auf eingeschaltet stellen
            }

          else if (Rasen_2_PrepareStart == HIGH && Verriegelung_Waessern == LOW) 
            {
            Rasen_2_WaessernActive = HIGH;
            Rasen_2_PrepareStart = LOW;
            Verriegelung_Waessern = HIGH;
            Restzeit_Rasen = (SetTime_Rasen* 60);
            Blynk.virtualWrite(V6, 255); //LED Rasen2 ein
            Blynk.virtualWrite(V16, HIGH); //Taster Rasen2 auf eingeschaltet stellen
            }

         else if (TopfPflanzen_1_PrepareStart == HIGH && Verriegelung_Waessern == LOW) 
            {
            TopfPflanzen_1_WaessernActive = HIGH;
            TopfPflanzen_1_PrepareStart = LOW;
            Verriegelung_Waessern = HIGH;
            Restzeit_Topfpflanzen = (SetTime_Topfpflanzen * 60);
            Blynk.virtualWrite(V7, 255); //LED Topfpflanzen1 ein
            Blynk.virtualWrite(V17, HIGH); //Taster Topfpflanzen 1 auf eingeschaltet stellen
            }
            
         else if (TopfPflanzen_2_PrepareStart == HIGH && Verriegelung_Waessern == LOW) 
            {
            TopfPflanzen_2_WaessernActive = HIGH;
            TopfPflanzen_2_PrepareStart = LOW;
            Verriegelung_Waessern = HIGH;
            Restzeit_Topfpflanzen = (SetTime_Topfpflanzen * 60);
            Blynk.virtualWrite(V8, 255); //LED Topfpflanzen2 ein
            Blynk.virtualWrite(V18, HIGH); //Taster Topfpflanzen2 auf eingeschaltet stellen
            }         
        }

    //****************************************************************************************************************************************************************************
    //********************************************************************** Automatikbetrieb ausschalten ************************************************************************
    // Wenn Restzeit im Automatikbetrieb abgelaufen ist aktive Bewässerung zurücksetzen
    //****************************************************************************************************************************************************************************
    //****************************************************************************************************************************************************************************
    if (((AutoOperation_Active == HIGH) && 
        (((Restzeit_Beete <= 0) && (Beet_1_WaessernActive == HIGH || Beet_2_WaessernActive == HIGH || Beet_3_WaessernActive == HIGH || Beet_4_WaessernActive == HIGH)) || 
        ((Restzeit_Rasen <= 0) && (Rasen_1_WaessernActive == HIGH || Rasen_2_WaessernActive == HIGH)) || 
        ((Restzeit_Topfpflanzen <= 0) &&  (TopfPflanzen_1_WaessernActive == HIGH || TopfPflanzen_2_WaessernActive == HIGH)) ||
        ((Restzeit_Wasserstellen <= 0) &&  (Wasserstelle_EinActive == HIGH)))) || 
        ((AutoOperation_Prepare == HIGH) && (Restzeit_Wasserstellen <= 0) &&  (Wasserstelle_EinActive == HIGH)))
        {
        // mit Verriegelung_Waessern auf low kann neue vorbereitete Bewässerung im nächsten Abschnitt gestartet werden
        Verriegelung_Waessern = LOW; 
        /*************************************************************************************************************************************************************************
         *  Bewässerungszonen im Automatikbetrieb bearbeiten
         *************************************************************************************************************************************************************************/
        if (Beet_1_WaessernActive == HIGH)
            {    
            Beet_1_WaessernActive = LOW;
            Beet_1_Gewaessert = HIGH;
            Blynk.virtualWrite(V1, 0); //LED V1 aus
            Blynk.virtualWrite(V11, HIGH); //Taster Beet1 auf eingeschaltet stellen
            String_Bewaesserungszeit_Beet1 = String(Bewaesserungszeit_Beete); //Für Informations E- Mail: Am Ende der Bewässerung eingestellte Zonen- Zeit in String schreiben 
            }
       
        if (Beet_2_WaessernActive == HIGH) 
            {
            Beet_2_WaessernActive = LOW;
            Beet_2_Gewaessert = HIGH;
            Blynk.virtualWrite(V2, 0); //LED V2 aus
            Blynk.virtualWrite(V12, HIGH); //Taster Beet2 auf eingeschaltet stellen
            String_Bewaesserungszeit_Beet2 = String(Bewaesserungszeit_Beete); //Für Informations E- Mail: Am Ende der Bewässerung eingestellte Zonen- Zeit in String schreiben 
            }
      
        if (Beet_3_WaessernActive == HIGH)
            {
            Beet_3_WaessernActive = LOW;
            Beet_3_Gewaessert = HIGH;
            Blynk.virtualWrite(V3, 0); //LED V3 aus
            Blynk.virtualWrite(V13, HIGH); //Taster Beet3 auf eingeschaltet stellen
            String_Bewaesserungszeit_Beet3 = String(Bewaesserungszeit_Beete); //Für Informations E- Mail: Am Ende der Bewässerung eingestellte Zonen- Zeit in String schreiben 
            }
    
        if (Beet_4_WaessernActive == HIGH)
            {
            Beet_4_WaessernActive = LOW;
            Beet_4_Gewaessert = HIGH;
            Blynk.virtualWrite(V4, 0); //LED V4 aus
            Blynk.virtualWrite(V14, HIGH); //Taster Beet4 auf eingeschaltet stellen
            String_Bewaesserungszeit_Beet4 = String(Bewaesserungszeit_Beete); //Für Informations E- Mail: Am Ende der Bewässerung eingestellte Zonen- Zeit in String schreiben 
            }
      
        if (Rasen_1_WaessernActive == HIGH)
            {
            Rasen_1_WaessernActive = LOW;
            Rasen_1_Gewaessert = HIGH;
            Blynk.virtualWrite(V5, 0); //LED V5 aus
            Blynk.virtualWrite(V15, HIGH); //Taster Rasen1 auf eingeschaltet stellen
            String_Bewaesserungszeit_Rasen1 = String(Bewaesserungszeit_Rasen); //Für Informations E- Mail: Am Ende der Bewässerung eingestellte Zonen- Zeit in String schreiben 
            }
      
        if (Rasen_2_WaessernActive == HIGH) 
            {
            Rasen_2_WaessernActive = LOW;
            Rasen_2_Gewaessert = HIGH;
            Blynk.virtualWrite(V6, 0); //LED V6 aus
            Blynk.virtualWrite(V16, HIGH); //Taster Rasen2 auf eingeschaltet stellen
            String_Bewaesserungszeit_Rasen2 = String(Bewaesserungszeit_Rasen); //Für Informations E- Mail: Am Ende der Bewässerung eingestellte Zonen- Zeit in String schreiben 
            }

        if (TopfPflanzen_1_WaessernActive == HIGH)
            {
            TopfPflanzen_1_WaessernActive = LOW;
            TopfPflanzen_1_Gewaessert = HIGH;
            Blynk.virtualWrite(V7, 0); //LED V7 aus
            Blynk.virtualWrite(V17, HIGH); //Taster Topf Pflanzen 1 auf eingeschaltet stellen
            String_Bewaesserungszeit_Topfpflanzen1 = String(Bewaesserungszeit_Topfpflanzen); //Für Informations E- Mail: Am Ende der Bewässerung eingestellte Zonen- Zeit in String schreiben 
             }
      
        if (TopfPflanzen_2_WaessernActive == HIGH)
            {
            TopfPflanzen_2_WaessernActive = LOW;
            TopfPflanzen_2_Gewaessert = HIGH;
            Blynk.virtualWrite(V8, 0); //LED V8 aus
            Blynk.virtualWrite(V18, HIGH); //Taster Topf Pflanzen 2 auf eingeschaltet stellen
            String_Bewaesserungszeit_Topfpflanzen2 = String(Bewaesserungszeit_Topfpflanzen); //Für Informations E- Mail: Am Ende der Bewässerung eingestellte Zonen- Zeit in String schreiben 
            }
       
        if (Wasserstelle_EinActive == HIGH) 
            {
             Wasserstelle_EinActive = LOW;
             Blynk.virtualWrite(V23, LOW); //Taster Wasserstellen auf ausgeschaltet stellen
             Blynk.setProperty(V23, "offLabel", "OFF"); //Taster Wasserstellen auf "OFF" stellen
            }
        }

     /*********************************************************************************************************************************************************************************************
     *********************************************************************** manueller Betrieb ***************************************************************************************************
          Angewählte Bewässerung im manuellen Modus einschalten
     **********************************************************************************************************************************************************************************************
     *********************************************************************************************************************************************************************************************/  
    if ((ManualOperation_Active == HIGH) && 
        (Beet_1_Manu_PrepareStart == HIGH || Beet_2_Manu_PrepareStart == HIGH || Beet_3_Manu_PrepareStart == HIGH || Beet_4_Manu_PrepareStart == HIGH || 
        Rasen_1_Manu_PrepareStart == HIGH || Rasen_2_Manu_PrepareStart == HIGH || TopfPflanzen_1_Manu_PrepareStart == HIGH || TopfPflanzen_2_Manu_PrepareStart == HIGH))
       {   
        if (Beet_1_Manu_PrepareStart == HIGH && Verriegelung_Waessern == LOW) 
           {
            Verriegelung_Waessern = HIGH;
            Beet_1_WaessernActive = HIGH;
            Restzeit_Beete = (SetTime_Beete * 60);
            Blynk.virtualWrite(V1, 255); //LED V1 ein
            }
            
        if (Beet_2_Manu_PrepareStart == HIGH && Verriegelung_Waessern == LOW) 
           {
            Verriegelung_Waessern = HIGH;
            Beet_2_WaessernActive = HIGH;
            Restzeit_Beete = (SetTime_Beete * 60);
            Blynk.virtualWrite(V2, 255); //LED V2 ein
            }
            
        if (Beet_3_Manu_PrepareStart == HIGH && Verriegelung_Waessern == LOW) 
           {
            Verriegelung_Waessern = HIGH;
            Beet_3_WaessernActive = HIGH;
            Restzeit_Beete = (SetTime_Beete * 60);
            Blynk.virtualWrite(V3, 255); //LED V3 ein
            }
           
          if (Beet_4_Manu_PrepareStart == HIGH && Verriegelung_Waessern == LOW) 
            {
            Verriegelung_Waessern = HIGH;
            Beet_4_WaessernActive = HIGH;
            Restzeit_Beete = (SetTime_Beete * 60);
            Blynk.virtualWrite(V4, 255); //LED V4 ein
            }

          if (Rasen_1_Manu_PrepareStart == HIGH && Verriegelung_Waessern == LOW) 
            {
            Verriegelung_Waessern = HIGH;
            Rasen_1_WaessernActive = HIGH;
            Restzeit_Rasen = (SetTime_Rasen * 60);
            Blynk.virtualWrite(V5, 255); //LED V5 ein
            }

          if (Rasen_2_Manu_PrepareStart == HIGH && Verriegelung_Waessern == LOW) 
            {
            Verriegelung_Waessern = HIGH;
            Rasen_2_WaessernActive = HIGH;
            Restzeit_Rasen = (SetTime_Rasen* 60);
            Blynk.virtualWrite(V6, 255); //LED V6 ein
            }

         if (TopfPflanzen_1_Manu_PrepareStart == HIGH && Verriegelung_Waessern == LOW) 
            {
            Verriegelung_Waessern = HIGH;
            TopfPflanzen_1_WaessernActive = HIGH;
            Restzeit_Topfpflanzen = (SetTime_Topfpflanzen * 60);
            Blynk.virtualWrite(V7, 255); //LED V7 ein
            }
            
         if (TopfPflanzen_2_Manu_PrepareStart == HIGH && Verriegelung_Waessern == LOW) 
            {
            Verriegelung_Waessern = HIGH;
            TopfPflanzen_2_WaessernActive = HIGH;
            Restzeit_Topfpflanzen = (SetTime_Topfpflanzen * 60);
            Blynk.virtualWrite(V8, 255); //LED V8 ein
            }         
      }

    /*********************************************************************************************************************************************************************************
     ***************************************************** Manueller Betrieb *********************************************************************************************************
         Wenn Restzeit im manuellen Betrieb abgelaufen ist aktive Bewässerung zurücksetzen
     *********************************************************************************************************************************************************************************
    **********************************************************************************************************************************************************************************/
    if ((ManualOperation_Active == HIGH) && (((Restzeit_Beete <= 0) && (Beet_1_Manu_PrepareStart == HIGH || Beet_2_Manu_PrepareStart == HIGH || Beet_3_Manu_PrepareStart == HIGH || 
          Beet_4_Manu_PrepareStart == HIGH)) || ((Restzeit_Rasen <= 0) && (Rasen_1_Manu_PrepareStart == HIGH || Rasen_2_Manu_PrepareStart == HIGH)) || 
        ((Restzeit_Topfpflanzen <= 0) &&  (TopfPflanzen_1_Manu_PrepareStart == HIGH || TopfPflanzen_2_Manu_PrepareStart == HIGH)) ||
        ((Restzeit_Wasserstellen <= 0) && (Wasserstelle_EinActive == HIGH))))
        {
        // mit Verriegelung_Waessern auf low kann neue vorbereitete Bewässerung im nächsten Abschnitt gestartet werden
        //Verriegelung_Waessern = LOW; 
        if ((Beet_1_Manu_PrepareStart == HIGH) && (Beet_1_WaessernActive == HIGH))
            {
              Beet_1_Manu_PrepareStart = LOW;
              Beet_1_WaessernActive = LOW;
              Blynk.virtualWrite(V1, 0); //LED V1 aus
              Blynk.virtualWrite(V11, LOW); //Taster Beet1 auf eingeschaltet stellen
              Blynk.setProperty(V11, "offLabel", "OFF"); //Taster Beet1 auf "ON" stellen
            }
      
        if ((Beet_2_Manu_PrepareStart == HIGH) && (Beet_2_WaessernActive == HIGH))
            {
              Beet_2_Manu_PrepareStart = LOW;
              Blynk.virtualWrite(V2, 0); //LED V2 aus
              Blynk.virtualWrite(V12, LOW); //Taster Beet1 auf eingeschaltet stellen
              Blynk.setProperty(V12, "offLabel", "OFF"); //Taster Beet2 auf "ON" stellen
            } 

         if (Beet_3_Manu_PrepareStart == HIGH)
            {
              Beet_3_Manu_PrepareStart = LOW;
              Blynk.virtualWrite(V3, 0); //LED V3 aus
              Blynk.virtualWrite(V13, LOW); //Taster Beet1 auf eingeschaltet stellen
              Blynk.setProperty(V13, "offLabel", "OFF"); //Taster Beet3 auf "ON" stellen
            }

            if (Beet_4_Manu_PrepareStart == HIGH)
            {
              Beet_4_Manu_PrepareStart = LOW;
              Blynk.virtualWrite(V4, 0); //LED V4 aus
              Blynk.virtualWrite(V14, LOW); //Taster Beet1 auf eingeschaltet stellen
              Blynk.setProperty(V14, "offLabel", "OFF"); //Taster Beet4 auf "ON" stellen
            }       

            if (Rasen_1_Manu_PrepareStart == HIGH)
            {
              Rasen_1_Manu_PrepareStart = LOW;
              Blynk.virtualWrite(V5, 0); //LED V5 aus
              Blynk.virtualWrite(V15, LOW); //Taster Beet1 auf eingeschaltet stellen
              Blynk.setProperty(V15, "offLabel", "OFF"); //Taster Rasen 1 auf "ON" stellen
             }       

            if (Rasen_2_Manu_PrepareStart == HIGH)
            {
              Rasen_2_Manu_PrepareStart = LOW;
              Blynk.virtualWrite(V6, 0); //LED V6 aus
              Blynk.virtualWrite(V16, LOW); //Taster Beet1 auf eingeschaltet stellen
              Blynk.setProperty(V16, "offLabel", "OFF"); //Taster Rasen 2 auf "ON" stellen
            }        

            if (TopfPflanzen_1_Manu_PrepareStart == HIGH)
            {
              TopfPflanzen_1_Manu_PrepareStart = LOW;
              Blynk.virtualWrite(V7, 0); //LED V6 aus
              Blynk.virtualWrite(V17, LOW); //Taster Beet1 auf eingeschaltet stellen
              Blynk.setProperty(V17, "offLabel", "OFF"); //Taster Topf Planzen 1 auf "ON" stellen
            }         
    
            if (TopfPflanzen_2_Manu_PrepareStart == HIGH)
            {
            TopfPflanzen_2_Manu_PrepareStart = LOW;
            Blynk.virtualWrite(V8, 0); //LED V6 aus
            Blynk.virtualWrite(V18, LOW); //Taster Beet1 auf ausgeschaltet stellen
            Blynk.setProperty(V18, "offLabel", "OFF"); //Taster Topf Pflanzen 2 auf "ON" stellen
            }         
        
            if ((Wasserstelle_EinActive == HIGH) && (ManualOperation_Active == HIGH))
            {
             Wasserstelle_EinActive = LOW;
             Blynk.virtualWrite(V23, LOW); //Taster Wasserstellen auf ausgeschaltet stellen
             Blynk.setProperty(V23, "offLabel", "OFF"); //Taster Wasserstellen auf "OFF" stellen
            }
        }

  /*********************************************************************************************************************************************************************************************
    Wenn Automatikbetrieb und manueller Betrieb ausgeschaltet sind die Einschalt- LED's für Beete (V1 bis V4) für Rasen (V5- V6) für Topfpflanzen (V7- V8) zurücksetzen
   *********************************************************************************************************************************************************************************************/
  /*if ((AutoOperation_Active == LOW) && (ManualOperation_Active == LOW))
    { 
     Blynk.virtualWrite(V1, 0);               //LED Beet1 aus [V1)]
     Blynk.virtualWrite(V2, 0);               //LED Beet 2 aus (V2)
     Blynk.virtualWrite(V3, 0);               //LED Beet 3 aus (V3)
     Blynk.virtualWrite(V4, 0);               //LED Beet 4 aus (V4)
     Blynk.virtualWrite(V5, 0);               //LED Rasen 1 aus (V5)
     Blynk.virtualWrite(V6, 0);               //LED Rasen 2 aus (V6)
     Blynk.virtualWrite(V7, 0);               //LED Topfpflanzen 1 aus (V7)
     Blynk.virtualWrite(V8, 0);               //LED Topfpflanzen 2 aus (V8)
    }
*/
    /*********************************************************************************************************************************************************************************
     *             Auswahl der Bewässerungszonen rücksetzen 
     *********************************************************************************************************************************************************************************/
    if (AutoOperation_Prepare == LOW && ManualOperation_Active == LOW && AnlageEntwaessern_Active == LOW) 
    {
      //Freigabe_Waesserungszeit_Lesen = HIGH; // Lesen der Bewässerungszeit sperren
      Beet_1_Manu_PrepareStart = LOW;
      Beet_1_WaessernActive = LOW;
      Beet_1_Gewaessert = LOW;  
      Blynk.virtualWrite(V1, 0);                        //LED V1 aus
      Blynk.virtualWrite(V11, LOW);                     //Taster Beet1 auf ausgeschaltet stellen
      Blynk.setProperty(V11, "offLabel", "OFF");        //Taster Beet1 auf "OFF" stellen

      Beet_2_Manu_PrepareStart = LOW;
      Beet_2_WaessernActive = LOW;
      Beet_2_Gewaessert = LOW;  
      Blynk.virtualWrite(V2, 0);                       //LED V2 aus
      Blynk.virtualWrite(V12, LOW);                    //Taster Beet2 auf ausgeschaltet stellen
      Blynk.setProperty(V12, "offLabel", "OFF");       //Taster Beet2 auf "OFF" stellen

      Beet_3_Manu_PrepareStart = LOW;
      Beet_3_WaessernActive = LOW;
      Beet_3_Gewaessert = LOW;  
      Blynk.virtualWrite(V3, 0);                        //LED V3 aus
      Blynk.virtualWrite(V13, LOW);                     //Taster Beet3 auf ausgeschaltet stellen
      Blynk.setProperty(V13, "offLabel", "OFF");        //Taster Beet3 auf "OFF" stellen

      Beet_4_Manu_PrepareStart = LOW;
      Beet_4_WaessernActive = LOW;
      Beet_4_Gewaessert = LOW;  
      Blynk.virtualWrite(V4, 0);                        //LED V4 aus
      Blynk.virtualWrite(V14, LOW);                     //Taster Beet4 auf ausgeschaltet stellen
      Blynk.setProperty(V14, "offLabel", "OFF");        //Taster Beet4 auf "OFF" stellen

      Rasen_1_Manu_PrepareStart = LOW;
      Rasen_1_WaessernActive = LOW;
      Rasen_1_Gewaessert = LOW;  
      Blynk.virtualWrite(V5, 0);                       //LED V5 aus
      Blynk.virtualWrite(V15, LOW);                    //Taster Rasen1 auf ausgeschaltet stellen
      Blynk.setProperty(V15, "offLabel", "OFF");       //Taster Rasen1 auf "OFF" stellen

      Rasen_2_Manu_PrepareStart = LOW;
      Rasen_2_WaessernActive = LOW;
      Rasen_2_Gewaessert = LOW;  
      Blynk.virtualWrite(V6, 0);                        //LED V6 aus
      Blynk.virtualWrite(V16, LOW);                     //Taster Rasen2 auf ausgeschaltet stellen
      Blynk.setProperty(V16, "offLabel", "OFF");        //Taster Rasen2 auf "OFF" stellen

      TopfPflanzen_1_Manu_PrepareStart = LOW;
      TopfPflanzen_1_WaessernActive = LOW;
      TopfPflanzen_1_Gewaessert = LOW;  
      Blynk.virtualWrite(V7, 0);                        //LED V7 aus
      Blynk.virtualWrite(V17, LOW);                     //Taster TopfPflanzen1 auf ausgeschaltet stellen
      Blynk.setProperty(V17, "offLabel", "OFF");        //Taster TopfPflanzen1 auf "OFF" stellen

      TopfPflanzen_2_Manu_PrepareStart = LOW;
      TopfPflanzen_2_WaessernActive = LOW;
      TopfPflanzen_2_Gewaessert = LOW;  
      Blynk.virtualWrite(V8, 0);                        //LED V8 aus
      Blynk.virtualWrite(V18, LOW);                     //Taster TopfPflanzen2 auf ausgeschaltet stellen
      Blynk.setProperty(V18, "offLabel", "OFF");        //Taster TopfPflanzen2 auf "OFF" stellen

    }

  /*************************************************************************************************************************************************
   * Anzeige Bewässerungspumpe eingeschaltet
   *************************************************************************************************************************************************/
  if (Bewaesserungspumpe == HIGH)
  {
       Blynk.virtualWrite(V19, 255); //LED V19 Bewässerungspumpe ein
  }
  if (Bewaesserungspumpe == LOW)
  {
       Blynk.virtualWrite(V19, 0); //LED V19 Bewässerungspumpe aus
  }
      
  /*************************************************************************************************************************************************
   * Anzeige Entwässerung der Anlage eingeschaltet
   *************************************************************************************************************************************************/
  if (AnlageEntwaessern_Active == HIGH)
  {
       Blynk.virtualWrite(V26, 255); //LED V26 Anlage Entwässern ein
       Blynk.virtualWrite(V1, 255); //LED V1 Ventil Beet1 entlüften
       Blynk.virtualWrite(V2, 255); //LED V2 Ventil Beet2 entlüften
       Blynk.virtualWrite(V3, 255); //LED V3 Ventil Beet3 entlüften
       Blynk.virtualWrite(V4, 255); //LED V4 Ventil Beet4 entlüften
       Blynk.virtualWrite(V5, 255); //LED V5 Ventil Rasen1 entlüften
       Blynk.virtualWrite(V6, 255); //LED V6 Ventil Rasen2 entlüften
       Blynk.virtualWrite(V7, 255); //LED V7 Ventil Töpfe1 entlüften
       Blynk.virtualWrite(V8, 255); //LED V8 Ventil Töpfe2 entlüften
    }
  
  else if((AnlageEntwaessern_Active == LOW) && (ManualOperation_Active == LOW) && (AutoOperation_Prepare == LOW) && (AutoOperation_Active == LOW))
  {
       Blynk.virtualWrite(V26, 0); //LED V26 Anlage Entwässern aus
       Blynk.virtualWrite(V1, 0); //LED V1 Ventil Beet1 schließen
       Blynk.virtualWrite(V2, 0); //LED V2 Ventil Beet2 schließen
       Blynk.virtualWrite(V3, 0); //LED V3 Ventil Beet3 schließen
       Blynk.virtualWrite(V4, 0); //LED V4 Ventil Beet4 schließen
       Blynk.virtualWrite(V5, 0); //LED V5 Ventil Rasen1 schließen
       Blynk.virtualWrite(V6, 0); //LED V6 Ventil Rasen2 schließen
       Blynk.virtualWrite(V7, 0); //LED V7 Ventil Töpfe1 schließen
       Blynk.virtualWrite(V8, 0); //LED V8 Ventil Töpfe2 schließen
  }

/******************************************************************************************************************************************************
 ************************************************************** Test *********************************************************************************/
  if (AutoOperation_Prepare == HIGH)
  {
      Blynk.virtualWrite(V50, 255); //LED V50 Anlage Entwässern ein
  }
  else{
       Blynk.virtualWrite(V50, 0); //LED V50 Anlage Entwässern aus
  }


if (AutoOperation_Active == HIGH)
  {
      Blynk.virtualWrite(V51, 255); //LED V51 Anlage Entwässern ein
  }
  else{
       Blynk.virtualWrite(V51, 0); //LED V51 Anlage Entwässern aus
  }

if (Beet_1_PrepareStart == HIGH)
  {
      Blynk.virtualWrite(V52, 255); //LED V52 Anlage Entwässern ein
  }
  else{
       Blynk.virtualWrite(V52, 0); //LED V52 Anlage Entwässern aus
  }

if (Wasserstelle_EinActive == HIGH)
  {
      Blynk.virtualWrite(V53, 255); //LED V53 Anlage Entwässern ein
  }
  else{
       Blynk.virtualWrite(V53, 0); //LED V53 Anlage Entwässern aus
  }

/*****************************************************************************************************************************************************
 ****************************************************************************************************************************************************  
  LCD- Anzeige setzen
  Wenn Restzeitangabe eine Stelle weniger anzeigen muss LCD- Anzeige löschen damit die nicht benutzte Stelle nicht mehr angezeigt wird
******************************************************************************************************************************************************
******************************************************************************************************************************************************/
  // Anzeige bei Übergang von 4 auf 3 von 3 auf 2 und von 2 auf 1 Stelle löschen
  if ((((Restzeit_Beete <= 1000 && Restzeit_Beete >= 995) || (Restzeit_Beete <= 100 && Restzeit_Beete >= 95) || (Restzeit_Beete <= 10 && Restzeit_Beete >= 5) || (Restzeit_Beete <= 5 && Restzeit_Beete >= 0)) &&
       (Beet_1_WaessernActive == HIGH || Beet_2_WaessernActive == HIGH || Beet_3_WaessernActive == HIGH || Beet_4_WaessernActive == HIGH)) || 
      (((Restzeit_Rasen <= 1000) && (Restzeit_Rasen >= 995) || (Restzeit_Rasen <= 100) && (Restzeit_Rasen >= 95) || (Restzeit_Rasen <= 10) && (Restzeit_Rasen >= 5) || (Restzeit_Rasen <= 5)&& (Restzeit_Rasen >= 0)) &&
       (Rasen_1_WaessernActive == HIGH || Rasen_2_WaessernActive == HIGH )) ||
      (((Restzeit_Topfpflanzen <= 1000) && (Restzeit_Topfpflanzen >= 995) || (Restzeit_Topfpflanzen <= 100) && (Restzeit_Topfpflanzen >= 95) || (Restzeit_Topfpflanzen <= 10) && (Restzeit_Topfpflanzen >= 5) || (Restzeit_Topfpflanzen <= 5) && (Restzeit_Topfpflanzen >= 0)) &&
       (TopfPflanzen_1_WaessernActive == HIGH || TopfPflanzen_2_WaessernActive == HIGH)))
     {      
      lcd.clear(); 
     }

  // Wenn die Restzeit < 0s ist zusätzlich zum Löschen des Displays Restzeit auf "0" setzen
  if (Beet_1_WaessernActive == LOW && Beet_2_WaessernActive == LOW && Beet_3_WaessernActive == LOW && Beet_4_WaessernActive == LOW)
     {
      Restzeit_Beete = 0;
     }

  if (Rasen_1_WaessernActive == LOW && Rasen_2_WaessernActive == LOW)
    {
       Restzeit_Rasen = 0;
    }

  if (TopfPflanzen_1_WaessernActive == LOW && TopfPflanzen_2_WaessernActive == LOW)
   {
     Restzeit_Topfpflanzen = 0;
   }

  //Bei der Aktivierung der Beetbewässerung  Displayanzeige löschen
  if ((Beet_1_WaessernActive || Beet_2_WaessernActive || Beet_3_WaessernActive || Beet_4_WaessernActive) && (PFL_BeeteActive == LOW))
    {
      PFL_BeeteActive = HIGH;
      lcd.clear(); 
    }
    //Display löschen  wieder freigeben
  if ((Beet_1_WaessernActive == LOW && Beet_2_WaessernActive == LOW && Beet_3_WaessernActive == LOW && Beet_4_WaessernActive == LOW))
     {
        PFL_BeeteActive = LOW;
     }

    //Bei der Aktivierung der Rasenbewässerung  Displayanzeige löschen
  if ((Rasen_1_WaessernActive || Rasen_2_WaessernActive ) && (PFL_RasenActive == LOW))
    {
      PFL_RasenActive = HIGH;
      lcd.clear(); 
    }

    //Display löschen wieder freigeben
  if ((Rasen_1_WaessernActive == LOW && Rasen_2_WaessernActive == LOW))
     {
        PFL_RasenActive = LOW;
     }

    //Bei der Aktivierung derTopfpflanzenbewässerung  Displayanzeige löschen
  if ((TopfPflanzen_1_WaessernActive || TopfPflanzen_2_WaessernActive ) && (PFL_TopfpflanzenActive == LOW))
    {
      PFL_TopfpflanzenActive = HIGH;
      lcd.clear(); 
    }
 
  //Display löschen wieder freigeben
  if ((TopfPflanzen_1_WaessernActive == LOW && TopfPflanzen_2_WaessernActive == LOW))
     {
        PFL_TopfpflanzenActive = LOW;
     }


  if (Beet_1_WaessernActive || Beet_2_WaessernActive || Beet_3_WaessernActive || Beet_4_WaessernActive)  
    {
    if ((Beet_1_WaessernActive == HIGH) && (ManualOperation_Active == HIGH))
      {
      lcd.print(0,0,"M_Beet1[s]:");
      }
     
  if ((Beet_1_WaessernActive == HIGH) && (AutoOperation_Prepare == HIGH))
      {
      lcd.print(0,0,"A_Beet1[s]:");
      }

    if ((Beet_2_WaessernActive == HIGH) && (ManualOperation_Active == HIGH))
      {
      lcd.print(0,0,"M_Beet2[s]:"); 
      }

    if ((Beet_2_WaessernActive == HIGH) && (AutoOperation_Prepare == HIGH))
      {
      lcd.print(0,0,"A_Beet2[s]:");
      }

    if ((Beet_3_WaessernActive == HIGH) && (ManualOperation_Active == HIGH))
      {
      lcd.print(0,0,"M_Beet3[s]:");
      }
      
    if ((Beet_3_WaessernActive == HIGH) && (AutoOperation_Prepare == HIGH))
      {
      lcd.print(0,0,"A_Beet3[s]:");
      }

    if ((Beet_4_WaessernActive == HIGH) && (ManualOperation_Active == HIGH))
      {
      lcd.print(0,0,"M_Beet4[s]:");
      }

    if ((Beet_4_WaessernActive == HIGH) && (AutoOperation_Prepare == HIGH))
      {
      lcd.print(0,0,"A_Beet4[s]:");
      }
      
      lcd.print(12,0,Bewaesserungszeit_Beete);
      lcd.print(0,1,"Rest[s]:");
      lcd.print(12,1,Restzeit_Beete);
    }

  if (Rasen_1_WaessernActive || Rasen_2_WaessernActive ) 
  {
    if ((Rasen_1_WaessernActive) &&  (ManualOperation_Active ))  
      {
      lcd.print(0,0,"M_Rasen1[s]:");
      }

    if ((Rasen_1_WaessernActive) && (AutoOperation_Prepare == HIGH))  
      {
        lcd.print(0,0,"A_Rasen1[s]:");
      }
    
    if ((Rasen_2_WaessernActive) &&  (ManualOperation_Active ))  
      {
      lcd.print(0,0,"M_Rasen2[s]:");
      }

    if ((Rasen_2_WaessernActive) && (AutoOperation_Prepare == HIGH))  
      {
      lcd.print(0,0,"A_Rasen2[s]:");
      }
  
      lcd.print(12,0,Bewaesserungszeit_Rasen);
      lcd.print(0,1,"Rest[s]:");
      lcd.print(12,1,Restzeit_Rasen);    
    }
    
  if (TopfPflanzen_1_WaessernActive || TopfPflanzen_2_WaessernActive)  
    {
      if ((TopfPflanzen_1_WaessernActive) && (ManualOperation_Active ))  
        {
        lcd.print(0,0,"M_Töpfe1[s]:");
        }

      if ((TopfPflanzen_1_WaessernActive) && (AutoOperation_Prepare ))  
        {
        lcd.print(0,0,"A_Töpfe1[s]:");
        }


    if ((TopfPflanzen_2_WaessernActive) && (ManualOperation_Active ))  
        {
        lcd.print(0,0,"M_Töpfe2[s]:");
        }

      if ((TopfPflanzen_2_WaessernActive) && (AutoOperation_Prepare ))  
        {
        lcd.print(0,0,"A_Töpfe2[s]:");
        }
        lcd.print(12,1,Restzeit_Topfpflanzen);
        lcd.print(0,1,"Rest[s]:");
        lcd.print(12,0,Bewaesserungszeit_Topfpflanzen);
      }

    // wenn kein Bewässerungsstrang eingeschaltet ist Bewässerungszeit des 1. Beetes anzeigen. Restzeit mit "0s" angeben
    if (Beet_1_WaessernActive == LOW && Beet_2_WaessernActive == LOW && Beet_3_WaessernActive == LOW && Beet_4_WaessernActive == LOW &&
      Rasen_1_WaessernActive == LOW && Rasen_2_WaessernActive == LOW && TopfPflanzen_1_WaessernActive == LOW && TopfPflanzen_2_WaessernActive == LOW)
    { 
      lcd.print(0,0,"  Beet1[s]:");
      lcd.print(12,0,Bewaesserungszeit_Beete);
      lcd.print(0,1,"Rest[s]:");
      lcd.print(12,1,"0   ");
    }

/******************************************************************************************************************************************************************************
 ******************************************************************************************************************************************************************************
  Mit Real- Time Clock die aktuelle Uhrzeit und Datum ermitteln
  ********************************************************************
  Digital clock display of the time
  You can call hour(), minute(), ... at any time
  Please see Time library examples for details
 ******************************************************************************************************************************************************************************/
  Current_hour = hour();                    //Aktuelle Uhrzeit Stunden
  Current_min = minute();                   //Aktuelle Uhrzeit Minuten
  Current_day = day();                      //Aktueller Tag (Datum)
  Current_weekday = weekday();              //Aktueller Wochentag (1= Sonntag/ 2=Montag/ 3=Dienstag.....7=Samstag)


/******************************************************************************************************************************************************************************
  LED- V22 zeigt an das die Bewässerungszeit für den Automatikbetrieb aktiv ist
 ******************************************************************************************************************************************************************************/
   if (AutoBewaesserungsZeitAktiv == HIGH){    
        Blynk.virtualWrite(V22, 255); //Rote LED Bewässerungszeit aktiv (V22) ein
    }
   else{
        Blynk.virtualWrite(V22, 0); //LED V22 aus
   }
/************************************************************************************
  LED- V20 zeigt an das der Automatikbetrieb aktiv ist
 ************************************************************************************/

     if (AutoOperation_Active == HIGH){    
        Blynk.virtualWrite(V20, 255); //LED V20 Automatikbetrieb ein
    }
   else{
        Blynk.virtualWrite(V20, 0); //LED V20 aus
   }
}

/**********************************************************************************************************************************************************************************
 ****************************************************************** Ende Blynk 5s task ********************************************************************************************
 **********************************************************************************************************************************************************************************/
 

BLYNK_CONNECTED() {
  // Synchronize time on connection
  rtc.begin();
}

/**************************************************************************************************************************************************************************
 **************************************************************************************************************************************************************************
 Mit Button Auto (V0) den automatischen- Modus vorbereiten
 Nur wenn der manuelle- oder der automatische- Betrieb aktiv ist kann die Bewässerung eingeschaltet werden
 **********************************************************************************************************
 Ein- ausschalten der Automatik- Funktion
***************************************************************************************************************************************************************************
***************************************************************************************************************************************************************************/
BLYNK_WRITE(V0)                                                   //Virtual Pin VO als Button zur Anwahl des Auto- Modes
{
  int Auto_Function = param.asInt(); // assigning incoming value from pin V0 to a variable
  if ((Auto_Function > 0) && (AutoOperation_Prepare == LOW))
  {
    Verriegelung_Waessern = LOW;
    AutoOperation_Prepare = HIGH;                     // Auto- Betrieb vorbereitet eingeschaltet Autozeit nicht aktiv
    Blynk.setProperty(V0, "onLabel", "ON");           //Taster Automatik auf "ON" stellen
    Blynk.virtualWrite(V0, HIGH);                     //Taster Autobetrieb einschalten
    Freigabe_Waesserungszeit_Lesen = HIGH;       // Lesen der Bewässerungszeit freigeben  

    // Wenn Automode eingeschaltet ist Manuellbetrieb ausschalten
    Blynk.virtualWrite(V9, 0);                        //LED V9 manueller Betrieb aus
    Blynk.virtualWrite(V10, LOW);                     //Taster Manuellbetrieb ausschalten
    Blynk.setProperty(V10, "offLabel", "OFF");        //Taster Manuell auf off setzen 
    Freigabe_Waesserungszeit_Lesen = LOW;        // Lesen der manuellen Bewässerungszeit sperren
    ManualOperation_Active = LOW;
  
    AnlageEntwaessern_Active = LOW;  
    Blynk.virtualWrite(V25, LOW); //Taster Wasserstelle auf ausgeschaltet stellen
    Blynk.setProperty(V25, "offLabel", "OFF"); //Taster Wasserstelle auf "Off" stellen
    Blynk.virtualWrite(V26, 0); 
  }

  else if ((Auto_Function <= 0) && (AutoOperation_Prepare == HIGH)) 
  {
    Freigabe_Waesserungszeit_Lesen = LOW; // Lesen der Bewässerungszeit sperren
    AutoOperation_Prepare = LOW; // Automatikbetrieb vorbereitet ein ausschalten
    AutoOperation_Active = LOW; // Automatikbetrieb ausschalten
    Blynk.setProperty(V0, "offLabel", "OFF"); // Taster Automatik auf "Off" stellen
    Blynk.virtualWrite(V0, LOW); // Schalter Automatikbetrieb auf "OFF" stellen
    Blynk.virtualWrite(V20, 0); // LED V20 automatischer Betrieb aus
  

    //Bei Automatik aus Vorwahl Bewässerung ausschalten
    Beet_1_PrepareStart = LOW;
    Beet_1_WaessernActive = LOW;
    Beet_1_Gewaessert = LOW;
    Blynk.virtualWrite(V11, LOW); //Taster Beet1 auf ausgeschaltet stellen
    Blynk.setProperty(V11, "offLabel", "OFF"); //Taster Beet1 auf "OFF" stellen
    Blynk.virtualWrite(V1, 0); //LED V1 aus
    
    Beet_2_PrepareStart = LOW;
    Beet_2_WaessernActive = LOW;
    Beet_2_Gewaessert = LOW;    
    Blynk.virtualWrite(V12, LOW); //Taster Beet2 auf ausgeschaltet stellen
    Blynk.setProperty(V12, "offLabel", "OFF"); //Taster Beet2 auf "OFF" stelle
    Blynk.virtualWrite(V2, 0); //LED V2 aus

    Beet_3_PrepareStart = LOW;
    Beet_3_WaessernActive = LOW;
    Beet_3_Gewaessert = LOW;
    Blynk.virtualWrite(V13, LOW); //Taster Beet3 auf ausgeschaltet stellen
    Blynk.setProperty(V13, "offLabel", "OFF"); //Taster Beet3 auf "OFF" stellen
    Blynk.virtualWrite(V3, 0); //LED V3 aus
    
    Beet_4_PrepareStart = LOW;
    Beet_4_WaessernActive = LOW;
    Beet_4_Gewaessert = LOW;
    Blynk.virtualWrite(V14, LOW); //Taster Beet4 auf ausgeschaltet stellen
    Blynk.setProperty(V14, "offLabel", "OFF"); //Taster Beet4 auf "OFF" stellen
    Blynk.virtualWrite(V4, 0); //LED V4 aus
    
    Rasen_1_PrepareStart = LOW;
    Rasen_1_WaessernActive = LOW;
    Rasen_1_Gewaessert = LOW;
    Blynk.virtualWrite(V15, LOW); //Taster Rasen1 auf ausgeschaltet stellen
    Blynk.setProperty(V15, "offLabel", "OFF"); //Taster Rasen1 auf "OFF" stelle
    Blynk.virtualWrite(V5, 0); //LED V5 aus
    
    Rasen_2_PrepareStart = LOW;
    Rasen_2_WaessernActive = LOW;
    Rasen_2_Gewaessert = LOW;
    Blynk.virtualWrite(V16, LOW); //Taster Rasen2 auf ausgeschaltet stellen
    Blynk.setProperty(V16, "offLabel", "OFF"); //Taster Rasen2 auf "OFF" stelle
    Blynk.virtualWrite(V6, 0); //LED V6 aus
  
    TopfPflanzen_1_PrepareStart = LOW;
    TopfPflanzen_1_WaessernActive = LOW;
    TopfPflanzen_1_Gewaessert = LOW;
    Blynk.virtualWrite(V17, LOW); //Taster TopfPflanzen 1 auf ausgeschaltet stellen
    Blynk.setProperty(V17, "offLabel", "OFF"); //Taster TopfPflanzen 1 auf "OFF" stellen
    Blynk.virtualWrite(V7, 0); //LED V7 aus
    
    TopfPflanzen_2_PrepareStart = LOW;
    TopfPflanzen_2_WaessernActive = LOW;
    TopfPflanzen_2_Gewaessert = LOW;    
    Blynk.virtualWrite(V18, LOW); //Taster TopfPflanzen 2 auf ausgeschaltet stellen
    Blynk.setProperty(V18, "offLabel", "OFF"); //Taster TopfPflanzen 2 auf "OFF" stellen
    Blynk.virtualWrite(V8, 0); //LED V8 aus  

    Wasserstelle_EinActive = LOW;
    Blynk.virtualWrite(V23, LOW); //Taster TopfPflanzen 2 auf ausgeschaltet stellen
    Blynk.setProperty(V23, "offLabel", "OFF"); //Taster TopfPflanzen 2 auf "OFF" stellen
  }   
}

/*************************************************************************************************************************************************************************************
 ********************************************************************Manuellen Betrieb einschalten **********************************************************************************
 Virtual Pin V1O als Button zur Anwahl des manuellen- Modes
 Nur wenn der manuelle- oder der automatische- Betrieb aktiv ist kann die Bewässerung eingeschaltet werden
 *************************************************************************************************************************************************************************************
*************************************************************************************************************************************************************************************/
BLYNK_WRITE(V10)                                                  
{
  int Manu_Function = param.asInt(); // assigning incoming value from pin V10 to a variable
   if ((Manu_Function > 0) && (ManualOperation_Active == LOW) && (AutoOperation_Prepare == LOW))
    {
      ManualOperation_Active = HIGH;
      Blynk.virtualWrite(V9, 255);                      //LED V9 manueller Betrieb ein
      Blynk.virtualWrite(V10, HIGH);                    //Taster Manuellbetrieb ausschalten
      Blynk.setProperty(V10, "onLabel", "ON");          //Taster Manuell auf off setzen
      Freigabe_Waesserungszeit_Lesen = HIGH;       // Freigebe zum Lesen der manuellen Bewässerungszeit
      Verriegelung_Waessern = LOW;

      AnlageEntwaessern_Active = LOW;  
      Blynk.virtualWrite(V25, LOW); //Taster Wasserstelle auf ausgeschaltet stellen
      Blynk.setProperty(V25, "offLabel", "OFF"); //Taster Wasserstelle auf "Off" stellen
      Blynk.virtualWrite(V26, 0); 

    // Wenn im manuellen Betrieb keine Bewässerungszone eingeschaltet ist wird im Display "0 " ausgegeben
    if (Beet_1_Manu_PrepareStart == LOW || Beet_2_Manu_PrepareStart == LOW || Beet_3_Manu_PrepareStart == LOW || Beet_4_Manu_PrepareStart == LOW || 
       Rasen_1_Manu_PrepareStart == LOW || Rasen_2_Manu_PrepareStart == LOW || TopfPflanzen_1_Manu_PrepareStart == LOW || TopfPflanzen_2_Manu_PrepareStart == LOW)
       {
       lcd.print(12,1,"0   ");
       }
    }
  
    //else if (((ManualOperation_Active == HIGH) && (Manu_Function <= 0)) || (AnlageEntwaessern_Active == HIGH)) //Manuellen Betrieb  ausschalteten 
    else if (((Manu_Function <= 0) && (ManualOperation_Active == HIGH)) || (AutoOperation_Prepare == HIGH)) //Manuellen Betrieb  ausschalteten 
    {
      Freigabe_Waesserungszeit_Lesen = LOW; // Lesen der manuellen Bewässerungszeit sperren
      ManualOperation_Active = LOW;
      Blynk.virtualWrite(V9, 0);                        //LED V9 manueller Betrieb aus
      Blynk.virtualWrite(V10, LOW);                     //Taster Manuellbetrieb ausschalten
      Blynk.setProperty(V10, "offLabel", "OFF");        //Taster Manuell auf off setzen      
    }
}

/***************************************************************************************************************************************************************************
 Beete und Rasenflächen einzeln im manuellen Betrieb anwählen 
 Es kann nur eine Bewässerungsfläche gewählt werden diese wird dann mit der eingestellten Zeit bewässert
 Während einer aktiven Bewässerung kann keine zweite Bewässeung eingeschaltet werden 
***********************************************************************************************************
 Beete und Rasenflächen nacheinander im automatischen Betrieb anwählen 
 Es können mehrere Bewässerungsfläche gewählt werden diese werden dann nacheinander mit der eingestellten Zeit bewässert
****************************************************************************************************************************************************************************/
//Virtual Pin V11 als Button zur Anwahl Beet 1
BLYNK_WRITE(V11)                                                  
{
  Beet_1_Choose = param.asInt(); // assigning incoming value from pin V11 to a variable
  if //Bedingungen für den manuellen Betrieb
    (((Beet_1_Choose > 0 && ManualOperation_Active == HIGH) && (Beet_1_Manu_PrepareStart == LOW && Beet_2_Manu_PrepareStart == LOW && Beet_3_Manu_PrepareStart == LOW && 
       Beet_4_Manu_PrepareStart == LOW  && Rasen_1_Manu_PrepareStart == LOW && Rasen_2_Manu_PrepareStart == LOW && 
       TopfPflanzen_1_Manu_PrepareStart == LOW && TopfPflanzen_2_Manu_PrepareStart == LOW )) ||
    //Bedingungen für den automatischen Betrieb
    ((Beet_1_Choose > 0) && (Beet_1_PrepareStart == LOW) && (AutoOperation_Prepare == HIGH)))
    { 
      //Freigabe_Waesserungszeit_Lesen = LOW; // Lesen der Bewässerungszeit sperren
      Restzeit_Beete = (SetTime_Beete * 60); 
      // Taster Beet1 auf ON stellen
      Blynk.virtualWrite(V11, HIGH); //Taster Beet1 auf eingeschaltet stellen
      Blynk.setProperty(V11, "onLabel", "ON"); //Taster Beet1 auf "ON" stellen

      if (ManualOperation_Active == HIGH)
        {
          Beet_1_Manu_PrepareStart = HIGH;
        }

      if (AutoOperation_Active == HIGH || AutoOperation_Prepare == HIGH)
        {
          Beet_1_PrepareStart = HIGH;
          //Verriegelung_Waessern = HIGH;
        }
      }
    
      else if ((Beet_1_Choose <= 0) || ((ManualOperation_Active == HIGH) &&  ((Beet_2_Choose > 0) || (Beet_3_Choose  > 0) || (Beet_4_Choose  > 0) ||
              (Rasen_1_Choose  > 0) || (Rasen_2_Choose  > 0) || (TopfPflanzen_1_Choose  > 0) || (TopfPflanzen_2_Choose  > 0))))
    {
      //Freigabe_Waesserungszeit_Lesen = HIGH; // Lesen der Bewässerungszeit sperren
      Verriegelung_Waessern       = LOW;
      Beet_1_Manu_PrepareStart    = LOW;
      Beet_1_WaessernActive       = LOW;
      Beet_1_Gewaessert           = LOW; 
      Beet_1_PrepareStart         = LOW; 
      Blynk.virtualWrite(V1, 0); //LED V1 aus
      Blynk.virtualWrite(V11, LOW); //Taster Beet1 auf ausgeschaltet stellen
      Blynk.setProperty(V11, "offLabel", "OFF"); //Taster Beet1 auf "OFF" stellen
    }
}  

BLYNK_WRITE(V12)                                                    
{
  Beet_2_Choose = param.asInt(); // assigning incoming value from pin V12 to a variable
  if //Bedingungen für den manuellen Betrieb
    (((Beet_2_Choose > 0 && ManualOperation_Active == HIGH) && (Beet_1_Manu_PrepareStart == LOW && Beet_2_Manu_PrepareStart == LOW && Beet_3_Manu_PrepareStart == LOW && 
       Beet_4_Manu_PrepareStart == LOW  && Rasen_1_Manu_PrepareStart == LOW && Rasen_2_Manu_PrepareStart == LOW && 
       TopfPflanzen_1_Manu_PrepareStart == LOW && TopfPflanzen_2_Manu_PrepareStart == LOW )) ||
    //Bedingungen für den automatischen Betrieb
    ((Beet_2_Choose > 0) && (Beet_2_PrepareStart == LOW) && (AutoOperation_Prepare == HIGH)))
    { 
      //Freigabe_Waesserungszeit_Lesen = LOW; // Lesen der Bewässerungszeit sperren
      Restzeit_Beete = (SetTime_Beete * 60); 
      // Taster Beet2 auf ON stellen
      Blynk.virtualWrite(V12, HIGH); //Taster Beet2 auf eingeschaltet stellen
      Blynk.setProperty(V12, "onLabel", "ON"); //Taster Beet2 auf "ON" stellen

      if (ManualOperation_Active == HIGH)
        {
          Beet_2_Manu_PrepareStart = HIGH;
        }

      if (AutoOperation_Active == HIGH || AutoOperation_Prepare == HIGH)
        {
          Beet_2_PrepareStart = HIGH;
          //Verriegelung_Waessern = HIGH;
        }
      }
    
      else if ((Beet_2_Choose <= 0) || ((ManualOperation_Active == HIGH) && ((Beet_1_Choose > 0) || (Beet_3_Choose > 0) || (Beet_4_Choose > 0) ||
              (Rasen_1_Choose > 0) || (Rasen_2_Choose > 0) || (TopfPflanzen_1_Choose > 0) || (TopfPflanzen_2_Choose > 0))))
    {
      //Freigabe_Waesserungszeit_Lesen = HIGH; // Lesen der Bewässerungszeit sperren
      Verriegelung_Waessern       = LOW;
      Beet_2_Manu_PrepareStart    = LOW;
      Beet_2_WaessernActive       = LOW;
      Beet_2_Gewaessert           = LOW;  
      Beet_2_PrepareStart         = LOW;
      Blynk.virtualWrite(V2, 0); //LED V2 aus
      Blynk.virtualWrite(V12, LOW); //Taster Beet2 auf ausgeschaltet stellen
      Blynk.setProperty(V12, "offLabel", "OFF"); //Taster Beet2 auf "OFF" stellen
    }
 }

 BLYNK_WRITE(V13)                                                    
{
  Beet_3_Choose = param.asInt(); // assigning incoming value from pin V13 to a variable
  if //Bedingungen für den manuellen Betrieb
    (((Beet_3_Choose > 0 && ManualOperation_Active == HIGH) && (Beet_1_Manu_PrepareStart == LOW && Beet_2_Manu_PrepareStart == LOW && Beet_3_Manu_PrepareStart == LOW && 
       Beet_4_Manu_PrepareStart == LOW  && Rasen_1_Manu_PrepareStart == LOW && Rasen_2_Manu_PrepareStart == LOW && 
       TopfPflanzen_1_Manu_PrepareStart == LOW && TopfPflanzen_2_Manu_PrepareStart == LOW )) ||
    //Bedingungen für den automatischen Betrieb
    ((Beet_3_Choose > 0) && (Beet_3_PrepareStart == LOW) && (AutoOperation_Prepare == HIGH)))
    { 
      //Freigabe_Waesserungszeit_Lesen = LOW; // Lesen der Bewässerungszeit sperren
      Restzeit_Beete = (SetTime_Beete * 60); 
      // Taster Beet3 auf ON stellen
      Blynk.virtualWrite(V13, HIGH); //Taster Beet3 auf eingeschaltet stellen
      Blynk.setProperty(V13, "onLabel", "ON"); //Taster Beet3 auf "ON" stellen

      if (ManualOperation_Active == HIGH)
        {
          Beet_3_Manu_PrepareStart = HIGH;
        }

      if (AutoOperation_Active == HIGH || AutoOperation_Prepare == HIGH)
        {
          Beet_3_PrepareStart = HIGH;
        }
      }
    
      else if ((Beet_3_Choose <= 0) || ((ManualOperation_Active == HIGH) && ((Beet_1_Choose > 0) || (Beet_2_Choose > 0) || (Beet_4_Choose > 0) ||
              (Rasen_1_Choose > 0) || (Rasen_2_Choose > 0) || (TopfPflanzen_1_Choose > 0) || (TopfPflanzen_2_Choose > 0))))
    {
      //Freigabe_Waesserungszeit_Lesen = HIGH; // Lesen der Bewässerungszeit sperren
      Verriegelung_Waessern       = LOW;
      Beet_3_Manu_PrepareStart    = LOW;
      Beet_3_WaessernActive       = LOW;
      Beet_3_Gewaessert           = LOW;  
      Beet_3_PrepareStart         = LOW; 
      Blynk.virtualWrite(V3, 0); //LED V3 aus
      Blynk.virtualWrite(V13, LOW); //Taster Beet3 auf ausgeschaltet stellen
      Blynk.setProperty(V13, "offLabel", "OFF"); //Taster Beet3 auf "OFF" stellen
    }
 }

  BLYNK_WRITE(V14)                                                    
{
  Beet_4_Choose = param.asInt(); // assigning incoming value from pin V14 to a variable
  if //Bedingungen für den manuellen Betrieb
    (((Beet_4_Choose > 0 && ManualOperation_Active == HIGH) && (Beet_1_Manu_PrepareStart == LOW && Beet_2_Manu_PrepareStart == LOW && Beet_3_Manu_PrepareStart == LOW && 
       Beet_4_Manu_PrepareStart == LOW  && Rasen_1_Manu_PrepareStart == LOW && Rasen_2_Manu_PrepareStart == LOW && 
       TopfPflanzen_1_Manu_PrepareStart == LOW && TopfPflanzen_2_Manu_PrepareStart == LOW )) ||
    //Bedingungen für den automatischen Betrieb
    ((Beet_4_Choose > 0) && (Beet_1_PrepareStart == LOW) && (AutoOperation_Prepare == HIGH)))
    { 
      //Freigabe_Waesserungszeit_Lesen = LOW; // Lesen der Bewässerungszeit sperren
      Restzeit_Beete = (SetTime_Beete * 60); 
      // Taster Beet4 auf ON stellen
      Blynk.virtualWrite(V14, HIGH); //Taster Beet4 auf eingeschaltet stellen
      Blynk.setProperty(V14, "onLabel", "ON"); //Taster Beet4 auf "ON" stellen

      if (ManualOperation_Active == HIGH)
        {
          Beet_4_Manu_PrepareStart = HIGH;
        }

      if (AutoOperation_Active == HIGH || AutoOperation_Prepare == HIGH)
        {
          Beet_4_PrepareStart = HIGH;
        }
      }
    
      else if ((Beet_4_Choose <= 0) || ((ManualOperation_Active == HIGH) && ((Beet_1_Choose > 0) || (Beet_2_Choose > 0) || (Beet_3_Choose > 0) ||
              (Rasen_1_Choose > 0) || (Rasen_2_Choose > 0) || (TopfPflanzen_1_Choose > 0) || (TopfPflanzen_2_Choose > 0))))
    {
      //Freigabe_Waesserungszeit_Lesen = HIGH; // Lesen der Bewässerungszeit sperren
      Verriegelung_Waessern       = LOW;
      Beet_4_Manu_PrepareStart    = LOW;
      Beet_4_WaessernActive       = LOW;
      Beet_4_Gewaessert           = LOW;  
      Beet_4_PrepareStart         = LOW; 
      Blynk.virtualWrite(V4, 0); //LED V4 aus
      Blynk.virtualWrite(V14, LOW); //Taster Beet4 auf ausgeschaltet stellen
      Blynk.setProperty(V14, "offLabel", "OFF"); //Taster Beet4 auf "OFF" stellen
    }
 }

  BLYNK_WRITE(V15)                                                    
{
  Rasen_1_Choose = param.asInt(); // assigning incoming value from pin V15 to a variable
  if //Bedingungen für den manuellen Betrieb
    (((Rasen_1_Choose > 0 && ManualOperation_Active == HIGH) && (Beet_1_Manu_PrepareStart == LOW && Beet_2_Manu_PrepareStart == LOW && Beet_3_Manu_PrepareStart == LOW && 
       Beet_4_Manu_PrepareStart == LOW  && Rasen_1_Manu_PrepareStart == LOW && Rasen_2_Manu_PrepareStart == LOW && 
       TopfPflanzen_1_Manu_PrepareStart == LOW && TopfPflanzen_2_Manu_PrepareStart == LOW )) ||
    //Bedingungen für den automatischen Betrieb
    ((Rasen_1_Choose > 0) && (Rasen_1_PrepareStart == LOW) && (AutoOperation_Prepare == HIGH)))
    { 
      //Freigabe_Waesserungszeit_Lesen = LOW; // Lesen der Bewässerungszeit sperren
      Restzeit_Rasen = (SetTime_Rasen * 60); 
      // Taster Rasen1 auf ON stellen
      Blynk.virtualWrite(V15, HIGH); //Taster Rasen1 auf eingeschaltet stellen
      Blynk.setProperty(V15, "onLabel", "ON"); //Taster Rasen1 auf "ON" stellen

      if (ManualOperation_Active == HIGH)
        {
          Rasen_1_Manu_PrepareStart = HIGH;
        }

      if (AutoOperation_Active == HIGH || AutoOperation_Prepare == HIGH)
        {
          Rasen_1_PrepareStart = HIGH;
        }
      }
    
      else if ((Rasen_1_Choose <= 0) || ((ManualOperation_Active == HIGH) && ((Beet_1_Choose > 0) || (Beet_2_Choose > 0) || (Beet_3_Choose > 0) ||
              (Beet_4_Choose > 0) || (Rasen_2_Choose > 0) || (TopfPflanzen_1_Choose > 0) || (TopfPflanzen_2_Choose > 0))))
    {
      //Freigabe_Waesserungszeit_Lesen = HIGH; // Lesen der Bewässerungszeit sperren
      Verriegelung_Waessern       = LOW;
      Rasen_1_Manu_PrepareStart    = LOW;
      Rasen_1_WaessernActive       = LOW;
      Rasen_1_Gewaessert           = LOW;  
      Rasen_1_PrepareStart         = LOW; 
      Blynk.virtualWrite(V5, 0); //LED V1 aus
      Blynk.virtualWrite(V15, LOW); //Taster Rasen1 auf ausgeschaltet stellen
      Blynk.setProperty(V15, "offLabel", "OFF"); //Taster Rasen1 auf "OFF" stellen
    }
 }

  BLYNK_WRITE(V16)                                                    
{
  Rasen_2_Choose = param.asInt(); // assigning incoming value from pin V16 to a variable
  if //Bedingungen für den manuellen Betrieb
    (((Rasen_2_Choose > 0 && ManualOperation_Active == HIGH) && (Beet_1_Manu_PrepareStart == LOW && Beet_2_Manu_PrepareStart == LOW && Beet_3_Manu_PrepareStart == LOW && 
       Beet_4_Manu_PrepareStart == LOW  && Rasen_1_Manu_PrepareStart == LOW && Rasen_2_Manu_PrepareStart == LOW && 
       TopfPflanzen_1_Manu_PrepareStart == LOW && TopfPflanzen_2_Manu_PrepareStart == LOW )) ||
    //Bedingungen für den automatischen Betrieb
    ((Rasen_2_Choose > 0) && (Rasen_2_PrepareStart == LOW) && (AutoOperation_Prepare == HIGH)))
    { 
      //Freigabe_Waesserungszeit_Lesen = LOW; // Lesen der Bewässerungszeit sperren
      Restzeit_Rasen = (SetTime_Rasen * 60); 
      // Taster Rasen2 auf ON stellen
      Blynk.virtualWrite(V16, HIGH); //Taster Rasen2 auf eingeschaltet stellen
      Blynk.setProperty(V16, "onLabel", "ON"); //Taster Rasen2 auf "ON" stellen

      if (ManualOperation_Active == HIGH)
        {
          Rasen_2_Manu_PrepareStart = HIGH;
        }

      if (AutoOperation_Active == HIGH || AutoOperation_Prepare == HIGH)
        {
          Rasen_2_PrepareStart = HIGH;
        }
      }
    
      else if ((Rasen_2_Choose <= 0) || ((ManualOperation_Active == HIGH) &&  ((Beet_1_Choose > 0) || (Beet_2_Choose > 0) || (Beet_3_Choose > 0) ||
              (Beet_4_Choose > 0) || (Rasen_1_Choose > 0) || (TopfPflanzen_1_Choose > 0) || (TopfPflanzen_2_Choose > 0))))
    {
      //Freigabe_Waesserungszeit_Lesen = HIGH; // Lesen der Bewässerungszeit sperren
      Verriegelung_Waessern       = LOW;
      Rasen_2_Manu_PrepareStart    = LOW;
      Rasen_2_WaessernActive       = LOW;
      Rasen_2_Gewaessert           = LOW; 
      Rasen_2_PrepareStart         = LOW;  
      Blynk.virtualWrite(V6, 0); //LED V1 aus
      Blynk.virtualWrite(V16, LOW); //Taster Rasen2 auf ausgeschaltet stellen
      Blynk.setProperty(V16, "offLabel", "OFF"); //Taster Rasen2 auf "OFF" stellen
    }
 }

 BLYNK_WRITE(V17)                                                    
{
  TopfPflanzen_1_Choose = param.asInt(); // assigning incoming value from pin V17 to a variable
  if //Bedingungen für den manuellen Betrieb
    (((TopfPflanzen_1_Choose > 0 && ManualOperation_Active == HIGH) && (Beet_1_Manu_PrepareStart == LOW && Beet_2_Manu_PrepareStart == LOW && Beet_3_Manu_PrepareStart == LOW && 
       Beet_4_Manu_PrepareStart == LOW  && Rasen_1_Manu_PrepareStart == LOW && Rasen_2_Manu_PrepareStart == LOW && 
       TopfPflanzen_1_Manu_PrepareStart == LOW && TopfPflanzen_2_Manu_PrepareStart == LOW )) ||
    //Bedingungen für den automatischen Betrieb
    ((TopfPflanzen_1_Choose > 0) && (TopfPflanzen_1_PrepareStart == LOW) && (AutoOperation_Prepare == HIGH)))
    { 
      //Freigabe_Waesserungszeit_Lesen = LOW; // Lesen der Bewässerungszeit sperren
      Restzeit_Topfpflanzen = (SetTime_Rasen * 60); 
      // Taster Topfpflanzen1 auf ON stellen
      Blynk.virtualWrite(V17, HIGH); //Taster Topfpflanzen1 auf eingeschaltet stellen
      Blynk.setProperty(V17, "onLabel", "ON"); //Taster Topfpflanzen1 auf "ON" stellen

      if (ManualOperation_Active == HIGH)
        {
          TopfPflanzen_1_Manu_PrepareStart = HIGH;
        }

      if (AutoOperation_Active == HIGH || AutoOperation_Prepare == HIGH)
        {
          TopfPflanzen_1_PrepareStart = HIGH;
        }
      }
    
      else if ((TopfPflanzen_1_Choose <= 0) ||  ((ManualOperation_Active == HIGH) &&  ((Beet_1_Choose > 0) || (Beet_2_Choose > 0) || (Beet_3_Choose > 0) ||
              (Beet_4_Choose > 0) || (Rasen_1_Choose > 0) || (Rasen_2_Choose > 0) || (TopfPflanzen_2_Choose > 0))))
    {
      //Freigabe_Waesserungszeit_Lesen = HIGH; // Lesen der Bewässerungszeit sperren
      Verriegelung_Waessern       = LOW;
      TopfPflanzen_1_Manu_PrepareStart    = LOW;
      TopfPflanzen_1_WaessernActive       = LOW;
      TopfPflanzen_1_Gewaessert           = LOW;  
      TopfPflanzen_1_PrepareStart         = LOW; 
      Blynk.virtualWrite(V7, 0); //LED V1 aus
      Blynk.virtualWrite(V17, LOW); //Taster TopfPflanzen1 auf ausgeschaltet stellen
      Blynk.setProperty(V17, "offLabel", "OFF"); //Taster TopfPflanzen1 auf "OFF" stellen
    }
 }

 BLYNK_WRITE(V18)                                                    
{
  TopfPflanzen_2_Choose = param.asInt(); // assigning incoming value from pin V18 to a variable
 if //Bedingungen für den manuellen Betrieb
    (((TopfPflanzen_2_Choose > 0 && ManualOperation_Active == HIGH) && (Beet_1_Manu_PrepareStart == LOW && Beet_2_Manu_PrepareStart == LOW && Beet_3_Manu_PrepareStart == LOW && 
       Beet_4_Manu_PrepareStart == LOW  && Rasen_1_Manu_PrepareStart == LOW && Rasen_2_Manu_PrepareStart == LOW && 
       TopfPflanzen_1_Manu_PrepareStart == LOW && TopfPflanzen_2_Manu_PrepareStart == LOW )) ||
    //Bedingungen für den automatischen Betrieb
    ((TopfPflanzen_2_Choose > 0) && (TopfPflanzen_2_PrepareStart == LOW) && (AutoOperation_Prepare == HIGH)))
    { 
      //Freigabe_Waesserungszeit_Lesen = LOW; // Lesen der Bewässerungszeit sperren
      Restzeit_Topfpflanzen = (SetTime_Rasen * 60); 
      // Taster Topfpflanzen2 auf ON stellen
      Blynk.virtualWrite(V18, HIGH); //Taster Topfpflanzen2 auf eingeschaltet stellen
      Blynk.setProperty(V18, "onLabel", "ON"); //Taster Topfpflanzen2 auf "ON" stellen

      if (ManualOperation_Active == HIGH)
        {
          TopfPflanzen_2_Manu_PrepareStart = HIGH;
        }

      if (AutoOperation_Active == HIGH || AutoOperation_Prepare == HIGH)
        {
          TopfPflanzen_2_PrepareStart = HIGH;
        }
      }
                                                                                      
      else if ((TopfPflanzen_2_Choose <= 0) || ((ManualOperation_Active == HIGH) &&  ((Beet_1_Choose > 0) || (Beet_2_Choose > 0) || (Beet_3_Choose > 0) ||
              (Beet_4_Choose > 0) || (Rasen_1_Choose > 0) || (Rasen_2_Choose > 0) || (TopfPflanzen_1_Choose > 0))))
    {
      //Freigabe_Waesserungszeit_Lesen = HIGH; // Lesen der Bewässerungszeit sperren
      Verriegelung_Waessern       = LOW;
      TopfPflanzen_2_Manu_PrepareStart    = LOW;
      TopfPflanzen_2_WaessernActive       = LOW;
      TopfPflanzen_2_Gewaessert           = LOW; 
      TopfPflanzen_2_PrepareStart         = LOW;  
      Blynk.virtualWrite(V8, 0); //LED V1 aus
      Blynk.virtualWrite(V18, LOW); //Taster TopfPflanzen1 auf ausgeschaltet stellen
      Blynk.setProperty(V18, "offLabel", "OFF"); //Taster TopfPflanzen1 auf "OFF" stellen
    }
 }

BLYNK_WRITE(V23)                                                    
{
  Wasserstelle_Choose = param.asInt(); // assigning incoming value from pin V18 to a variable
  if ((Wasserstelle_Choose > 0) && (ManualOperation_Active == HIGH || AutoOperation_Active == HIGH || AutoOperation_Prepare == HIGH))
      {       
       Restzeit_Wasserstellen = (SetTime_Wasserstellen * 60);  
       Wasserstelle_EinActive = HIGH; //Wasserstelle Einschalten
       Blynk.virtualWrite(V23, HIGH); //Taster Wasserstelle auf eingeschaltet stellen
       Blynk.setProperty(V23, "onLabel", "ON"); //Taster Wasserstelle auf "ON" stellen
      }
    
    //else //  2 nicht gewählt
     if ((Wasserstelle_Choose <= 0) || (ManualOperation_Active == LOW && AutoOperation_Active == LOW && AutoOperation_Prepare == LOW))
      {
       Wasserstelle_EinActive = LOW;  
       Blynk.virtualWrite(V23, LOW); //Taster Wasserstelle auf ausgeschaltet stellen
       Blynk.setProperty(V23, "offLabel", "OFF"); //Taster Wasserstelle auf "Off" stellen
      } 
 }

BLYNK_WRITE(V25)                                                    
{
  Entwaessern_Choose = param.asInt(); // assigning incoming value from pin V18 to a variable
  if (Entwaessern_Choose > 0 && (ManualOperation_Active == LOW && AutoOperation_Active == LOW && AutoOperation_Prepare == LOW))
      {       
       AnlageEntwaessern_Active = HIGH; //Wasserstelle Einschalten
        // Taster TopfPflanzen 2 auf ON stellen
       Blynk.virtualWrite(V25, HIGH); //Taster Wasserstelle auf eingeschaltet stellen
       Blynk.setProperty(V25, "onLabel", "ON"); //Taster Wasserstelle auf "ON" stellen
      }
    else //  2 nicht gewählt
      {
       AnlageEntwaessern_Active = LOW;  
       Blynk.virtualWrite(V25, LOW); //Taster Wasserstelle auf ausgeschaltet stellen
       Blynk.setProperty(V25, "offLabel", "OFF"); //Taster Wasserstelle auf "Off" stellen
      } 
 }

BLYNK_WRITE(V30){
  //Serial.print("Got a value: ");
  //Serial.println(param.asStr());
}
/*****************************************************************************************************************
  Mit Funktion "Set Time" Wochentag und Zeit für die Bewässerung einstellen
  In der Funktion Automatikmodus wird in dieser Zeit die Bewässerung durchgeführt
 *****************************************************************************************************************/
BLYNK_WRITE(V21) {
  long startTimeInSecs = param[0].asLong();
  Serial.println(startTimeInSecs);
  Serial.println();
  TimeInputParam t(param);
  
  SThour = t.getStartHour(); // eingestellte Startzeit [h]
  STmin = t.getStartMinute(); // eingestellte Startzeit [min]
  STsec = t.getStartSecond(); // eingestellte Startzeit [sec]
  SPhour = t.getStopHour(); // eingestellte Stopzeit [h]
  SPmin = t.getStopMinute(); // eingestellte Stopzeit [min]
  SPsec = t.getStopSecond(); // eingestellte Stopzeit [sec]

  /************************************************************************************************
   Abfrage ob Wochentag in der APP gewählt ist
   Für Montag wird eine "1" für Dienstag eine "2" ...... für Sonntag eine "7" in "i" zurückgemeldet
  *************************************************************************************************/ 
  Monday_Choose     = LOW;
  Tuesday_Choose    = LOW;
  Wednesday_Choose  = LOW;
  Thursday_Choose   = LOW;
  Friday_Choose     = LOW;
  Saturday_Choose   = LOW;
  Sunday_Choose     = LOW;

  for (int i = 1; i <= 7; i++) {   
    //***************************************Montag gewählt?************************************************ 
    if ((t.isWeekdaySelected(i)== HIGH) && (i == 1)){
       Monday_Choose = HIGH;
    }
    else if ((t.isWeekdaySelected(i)== LOW) && (i == 1)) {
       Monday_Choose = LOW;
    }

    //*************************************Dienstag gewählt? **********************************************
    //Tuesday_Choose = LOW;
    if ((t.isWeekdaySelected(i)== HIGH) && (i == 2)){
        Tuesday_Choose = HIGH;
      }
      else if ((t.isWeekdaySelected(i)== LOW) && (i == 2)) {
        Tuesday_Choose = LOW;
      }

    //*************************************Mittwoch gewählt *************************************************
     if ((t.isWeekdaySelected(i)== HIGH) && (i == 3)){
        Wednesday_Choose = HIGH;
         
      }
      else if ((t.isWeekdaySelected(i)== LOW) && (i == 3)) {
        Wednesday_Choose = LOW; 
      }

    //************************************** Donnerstag gewählt? ********************************************
    if ((t.isWeekdaySelected(i)== HIGH) && (i == 4)){
       Thursday_Choose = HIGH;
      }
      else if ((t.isWeekdaySelected(i)== LOW) && (i == 4)) {
       Thursday_Choose = LOW;
      }

  //************************************** Freitag gewählt? ********************************************
    if ((t.isWeekdaySelected(i)== HIGH) && (i == 5)){
        Friday_Choose = HIGH;
      }
      else if ((t.isWeekdaySelected(i)== LOW) && (i == 5)) {
        Friday_Choose = LOW;
      }

     //************************************** Samstag gewählt? ********************************************
    if ((t.isWeekdaySelected(i)== HIGH) && (i == 6)){
        Saturday_Choose = HIGH;
      }
      else if ((t.isWeekdaySelected(i)== LOW) && (i == 6)) {
        Saturday_Choose = LOW;
      }
 
    //************************************** Sonntag gewählt? ********************************************
    if ((t.isWeekdaySelected(i)== HIGH) && (i == 7)){
       Sunday_Choose = HIGH;  
      }
      else if ((t.isWeekdaySelected(i)== LOW) && (i == 7)) {
       Sunday_Choose = LOW;   
      }
   }
 }


// Vorwahl der Bewässerungszeit für Beete einstellen (V101)
BLYNK_WRITE(V101)
{
   SetTime_Beete = param.asInt(); // assigning incoming value from pin V101 to a variable
   //Serial.println(SetTime_Beete); 
   Bewaesserungszeit_Beete = (SetTime_Beete * 60);
   Restzeit_Beete = (SetTime_Beete * 60);
   lcd.clear();
}

// Vorwahl der Bewässerungszeit für Rasen einstellen (V102)
BLYNK_WRITE(V102)
{
   SetTime_Rasen = param.asInt(); // assigning incoming value from pin V102 to a variable
   Bewaesserungszeit_Rasen = (SetTime_Rasen * 60);
   Restzeit_Rasen = (SetTime_Rasen * 60);
   lcd.clear();
}

// Vorwahl der Bewässerungszeit für die Topfpflanzen einstellen (V103)
BLYNK_WRITE(V103)
{
   SetTime_Topfpflanzen = param.asInt(); // assigning incoming value from pin V102 to a variable
   Bewaesserungszeit_Topfpflanzen = (SetTime_Topfpflanzen * 60);
   Restzeit_Topfpflanzen = (SetTime_Topfpflanzen * 60);
   lcd.clear();
}

// Vorwahl der Bewässerungszeit für die Wasserstellen einstellen
BLYNK_WRITE(V104)
{
   SetTime_Wasserstellen = param.asInt(); // assigning incoming value from pin V104 to a variable
   Bewaesserungszeit_Wasserstellen = (SetTime_Wasserstellen * 60);
   Restzeit_Wasserstellen = (SetTime_Wasserstellen * 60);
   //lcd.clear();
}


void setup()
{
  
 /************************************************************************************************************************
  * digitale Pins zuordnen
  ************************************************************************************************************************/
  pinMode(GPIO_2, OUTPUT);
  
  pinMode(GPIO_4, OUTPUT);
  
  pinMode(GPIO_5, OUTPUT);

  //pinMode(GPIO_12, OUTPUT);

  //pinMode(GPIO_13, OUTPUT);

  //pinMode(GPIO_14, OUTPUT);

  //pinMode(GPIO_15, OUTPUT);

  pinMode(GPIO_16, OUTPUT);

  pinMode(GPIO_17, OUTPUT);

  pinMode(GPIO_18, OUTPUT);

  pinMode(GPIO_19, OUTPUT);

  pinMode(GPIO_21, OUTPUT);

  pinMode(GPIO_22, OUTPUT);
   
  pinMode(GPIO_23, OUTPUT);

  pinMode(GPIO_25, OUTPUT);
   
  pinMode(GPIO_26, OUTPUT);

  //pinMode(LED_BUILTIN, OUTPUT); // LED als Output definieren
  //digitalWrite(LED_BUILTIN, HIGH); // Ausschalten
  
  BewaesserungsStart = HIGH; // Variable für ersten Anlauf
  // Debug console
  //Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);

  //Zeisynchronisation 
  setSyncInterval(10 * 60); // Sync interval in seconds (10 minutes)
  // Display digital clock every 5 seconds
  timer.setInterval(5000L, clockDisplay);
}

/**************************************************************************************************************************************************************************************
 ************************************************************************* Loop Funktionen ********************************************************************************************
 **************************************************************************************************************************************************************************************/
void loop()
{  
  Blynk.run();
  timer.run();

  /*************************************************************************************************************************************************************************************
    * Überprüfen ob der aktuelle Wochentag für die Bewässerung gewählt wurde 
    ************************************************************************************************************************************************************************************/
    Current_WeekdayIsChoose = LOW; //Vor Überprüfung Tagesauswahl zurück setzen
    //Überprüfung am Montag
    if ((Current_weekday == 2) && (Monday_Choose == HIGH)){ //Current_weekday startet mit "1" für Sonntag und endet mit einer "7" als Rückmeldung für Samstag
       Current_WeekdayIsChoose = HIGH;
    }
    //Überprüfung am Dienstag
    else if ((Current_weekday == 3) && (Tuesday_Choose == HIGH)){
       Current_WeekdayIsChoose = HIGH;
    }
    //Überprüfung am Mittwoch 
    else if ((Current_weekday == 4) && (Wednesday_Choose == HIGH)){
       Current_WeekdayIsChoose = HIGH;
    }
    //Überprüfung am Donnerstag
    else if ((Current_weekday == 5) && (Thursday_Choose == HIGH)){
       Current_WeekdayIsChoose = HIGH;
    }
    //Überprüfung am Freitag
    else if ((Current_weekday == 6) && (Friday_Choose == HIGH)){
       Current_WeekdayIsChoose = HIGH;
    }
    //Überprüfung am Samstag
    else if ((Current_weekday == 7) && (Saturday_Choose == HIGH)){
       Current_WeekdayIsChoose = HIGH;
    }
    //Überprüfung am Sonntag
    else if ((Current_weekday == 1) && (Sunday_Choose == HIGH)){
       Current_WeekdayIsChoose = HIGH;
    }

//*************************************************************************************************************************************************************************************
// Innerhalb des Bewässerungszeitraums Variable setzen und LED- einschalten
//*************************************************************************************************************************************************************************************
  if ((Current_hour >= SThour) && (Current_min >= STmin) && (AutoBewaesserungsZeitAktiv == LOW) && (Current_WeekdayIsChoose == HIGH)){
      AutoBewaesserungsZeitAktiv = HIGH;
  }

  if ((((Current_hour >= SPhour) && (Current_min >= SPmin)) || (Current_hour > SPhour) || (Current_WeekdayIsChoose == LOW)) && (AutoBewaesserungsZeitAktiv == HIGH)){
     AutoBewaesserungsZeitAktiv = LOW;
    
    //Bewässerungen für den nächsten Automatikbetrieb wieder vorbereiten
     if (Beet_1_WaessernActive == HIGH) {
      Beet_1_PrepareStart = HIGH;
      Beet_1_WaessernActive = LOW;
     }
      if (Beet_2_WaessernActive == HIGH) {
      Beet_2_PrepareStart = HIGH;
      Beet_2_WaessernActive = LOW;
     }
      if (Beet_3_WaessernActive == HIGH) {
      Beet_3_PrepareStart = HIGH;
      Beet_3_WaessernActive = LOW;
     }
    if (Beet_4_WaessernActive == HIGH) {
      Beet_4_PrepareStart = HIGH;
      Beet_4_WaessernActive = LOW;
     }
     if (Rasen_1_WaessernActive == HIGH) {
      Rasen_1_PrepareStart = HIGH;
      Rasen_1_WaessernActive = LOW;
     } 
      if (Rasen_2_WaessernActive == HIGH) {
      Rasen_2_PrepareStart = HIGH;
      Rasen_2_WaessernActive = LOW;
     } 
     if (TopfPflanzen_1_WaessernActive == HIGH) {
      TopfPflanzen_1_PrepareStart = HIGH;
      TopfPflanzen_1_WaessernActive = LOW;
     } 
     if (TopfPflanzen_2_WaessernActive == HIGH){
      TopfPflanzen_2_PrepareStart = HIGH;
      TopfPflanzen_2_WaessernActive = LOW;
     }  
    }

  /***************************************************************************************************************************************************
            Zeitfenster für Auto- Betrieb aktivieren 
            In diesem Zeitfenster wird der "vorbereitete Automatikbetrieb" auf "Automatikbetrieb" gesetzt
  ****************************************************************************************************************************************************/
  if (AutoOperation_Prepare == HIGH && AutoBewaesserungsZeitAktiv == HIGH){
    AutoOperation_Active = HIGH;
    AutoOperation_Prepare = HIGH; 
    }

  /***************************************************************************************************************************************************
   Zeitfenster für Auto- Betrieb abgelaufen
  ****************************************************************************************************************************************************/
  if (AutoOperation_Active == HIGH && AutoBewaesserungsZeitAktiv == LOW){
     AutoOperation_Active = LOW;
     AutoOperation_Prepare = HIGH;

      //Nach Ablauf der Bewässerungsfreigabezeit gewählte Bewässerungsflächen wieder freigeben für den nächsten Bewässerungszeitraum
     if (Beet_1_Choose == HIGH){
          Beet_1_PrepareStart = HIGH;
          Beet_1_WaessernActive = LOW;
          Beet_1_Gewaessert = LOW;
        }
     if (Beet_2_Choose == HIGH){
          Beet_2_PrepareStart = HIGH;
          Beet_2_WaessernActive = LOW;
          Beet_2_Gewaessert = LOW;
        }
     if (Beet_3_Choose == HIGH){
          Beet_3_PrepareStart = HIGH;
          Beet_3_WaessernActive = LOW;
          Beet_3_Gewaessert = LOW;
        }
     if (Beet_4_Choose == HIGH){
          Beet_4_PrepareStart = HIGH;
          Beet_4_WaessernActive = LOW;
          Beet_4_Gewaessert = LOW;
        }
     if (Rasen_1_Choose == HIGH){ 
          Rasen_1_PrepareStart = HIGH;   
          Rasen_1_WaessernActive = LOW;
          Rasen_1_Gewaessert = LOW;
        }
      if (Rasen_2_Choose == HIGH){ 
          Rasen_2_PrepareStart = HIGH;   
          Rasen_2_WaessernActive = LOW;
          Rasen_2_Gewaessert = LOW;
        }
      if (TopfPflanzen_1_Choose == HIGH){ 
          TopfPflanzen_1_PrepareStart = HIGH; 
          TopfPflanzen_1_WaessernActive = LOW;
          TopfPflanzen_1_Gewaessert = LOW;
      }
      if (TopfPflanzen_2_Choose == HIGH){ 
          TopfPflanzen_2_PrepareStart = HIGH; 
          TopfPflanzen_2_WaessernActive = LOW;
          TopfPflanzen_2_Gewaessert = LOW;
      }  
    }

    /*****************************************************************************************************************************************************
     * Positive Flanke wenn die Autozeit aktiviert wird
     ****************************************************************************************************************************************************/
    if ((PFG_Merker_AutoOperation_Active == LOW) && (AutoBewaesserungsZeitAktiv == HIGH))
    { 
      PFG_Merker_AutoOperation_Active = HIGH;
      PFL_AutoOperation_Active = HIGH; // high_Flanke bei Autozeit "EIN" für einen Durchlauf
    }
    else
    {
      PFL_AutoOperation_Active = LOW;
    }
    
    if (AutoBewaesserungsZeitAktiv == LOW)
    {
      PFG_Merker_AutoOperation_Active = LOW;   
    }

    /***************************************************************************************************************************************************************
     Im 1 Durchlauf nach Aktivierung des Automatikmodusses alle Strings der Bewässerungszeiten die per Mail übertragen werden auf "0" s setzen
    ****************************************************************************************************************************************************************/
    if (PFL_AutoOperation_Active == HIGH )
    {
      // Rücksetzen der Bewässerungszeit auf 0s wenn der Autobetrieb eingeschaltet wird
      String_Bewaesserungszeit_Beet1 = "0";   //Für Informations E- Mail Bewässerungszeit in String schreiben 
      String_Bewaesserungszeit_Beet2 = "0";   //Für Informations E- Mail Bewässerungszeit in String schreiben 
      String_Bewaesserungszeit_Beet3 = "0";   //Für Informations E- Mail Bewässerungszeit in String schreiben 
      String_Bewaesserungszeit_Beet4 = "0";   //Für Informations E- Mail Bewässerungszeit in String schreiben 
      String_Bewaesserungszeit_Rasen1 = "0";  //Für Informations E- Mail Bewässerungszeit in String schreiben 
      String_Bewaesserungszeit_Rasen2 = "0";  //Für Informations E- Mail Bewässerungszeit in String schreiben 
      String_Bewaesserungszeit_Topfpflanzen1 = "0";  //Für Informations E- Mail Bewässerungszeit in String schreiben 
      String_Bewaesserungszeit_Topfpflanzen2 = "0";  //Für Informations E- Mail Bewässerungszeit in String schreiben 
    }

  /*****************************************************************************************************************************************************
     * Negative Flanke wenn die Autozeit deaktiviert wird
     ****************************************************************************************************************************************************/
    if ((NFG_Merker_AutoOperation_Active == LOW) && (AutoBewaesserungsZeitAktiv == LOW))
    { 
      NFG_Merker_AutoOperation_Active = HIGH;
      NFL_AutoOperation_Active = HIGH;
    }
    else
    {
      NFL_AutoOperation_Active = LOW;
    }
    
    if (AutoBewaesserungsZeitAktiv == HIGH)
    {
       NFG_Merker_AutoOperation_Active = LOW;
    }

    /******************************************************************************************************************************************************************
     * Im 1. Durchlauf nach dem Abschalten des Automatikmodusses Bewässerungsstring für die Email zusammensetzen und verschicken
     ******************************************************************************************************************************************************************/
    if (NFL_AutoOperation_Active == HIGH)
    {
        //char Mail_str[250];
        Mail_str = "";
        Mail_str = "Bewässerungszeiten im letzten Automatikbetrieb: ";
        Mail_str = Mail_str + " Bewässerungszeit Beet1: ";
        Mail_str = Mail_str + String_Bewaesserungszeit_Beet1 + "s";   
        Mail_str = Mail_str + " Bewässerungszeit Beet2: ";
        Mail_str = Mail_str + String_Bewaesserungszeit_Beet2 + "s";  
        Mail_str = Mail_str + " Bewässerungszeit Beet3: ";
        Mail_str = Mail_str + String_Bewaesserungszeit_Beet3 + "s";   
        Mail_str = Mail_str + " Bewässerungszeit Beet4: ";
        Mail_str = Mail_str + String_Bewaesserungszeit_Beet4 + "s";  
        Mail_str = Mail_str + " Bewässerungszeit Rasen1: ";
        Mail_str = Mail_str + String_Bewaesserungszeit_Rasen1 + "s";   
        Mail_str = Mail_str + " Bewässerungszeit Rasen2: ";
        Mail_str = Mail_str + String_Bewaesserungszeit_Rasen2 + "s";  
        Mail_str = Mail_str + " Bewässerungszeit Topfpflanzen1: ";
        Mail_str = Mail_str + String_Bewaesserungszeit_Topfpflanzen1 + "s";   
        Mail_str = Mail_str + " Bewässerungszeit Topfpflanzen2: ";
        Mail_str = Mail_str + String_Bewaesserungszeit_Topfpflanzen2 + "s";  

 
        //Blynk.email("michaelhams1959@gmail.com",  Mail_str);
        Blynk.email("michaelhams@web.de",  Mail_str);
    }

    /******************************************************************************************************************************************************************
     Relais einschalten wenn der Bewässerungsstrang aktiviert ist (GPIO muss auf GND (low) geschaltet werden) 
    *******************************************************************************************************************************************************************/ 
  if (Beet_1_WaessernActive == HIGH){
      digitalWrite(GPIO_2, LOW);
      }
      else{
      digitalWrite(GPIO_2, HIGH);
      }

    if (Beet_2_WaessernActive == HIGH){
      digitalWrite(GPIO_4, LOW);
      }
      else{
      digitalWrite(GPIO_4, HIGH);
      }

   if (Beet_3_WaessernActive == HIGH){
      digitalWrite(GPIO_5, LOW);
      }
      else{
      digitalWrite(GPIO_5, HIGH);
      }

    if (Beet_4_WaessernActive == HIGH){
      digitalWrite(GPIO_18, LOW);
      }
      else{
      digitalWrite(GPIO_18, HIGH);
      }
      
   if (Rasen_1_WaessernActive == HIGH){
      digitalWrite(GPIO_19, LOW);
      }
      else{
      digitalWrite(GPIO_19, HIGH);
      }
      
   if (Rasen_2_WaessernActive == HIGH){
      digitalWrite(GPIO_21, LOW);
      }
      else{
      digitalWrite(GPIO_21, HIGH);
      }

   if (TopfPflanzen_1_WaessernActive == HIGH){
      digitalWrite(GPIO_17, LOW);
      }
      else{
      digitalWrite(GPIO_17, HIGH);
      }
      
  if (TopfPflanzen_2_WaessernActive == HIGH){
      digitalWrite(GPIO_16, LOW);
      }
      else{
      digitalWrite(GPIO_16, HIGH);
      }

  if (Wasserstelle_EinActive == HIGH){
      digitalWrite(GPIO_22, LOW);
      }
      else{
      digitalWrite(GPIO_22, HIGH);
      }

/**********************************************************************************************************************************************
 * Wenn eine Wässerung aktiv ist Ausgang Wasserpumpe einschalten
 **********************************************************************************************************************************************/
   if (Beet_1_WaessernActive == HIGH ||Beet_2_WaessernActive == HIGH || Beet_3_WaessernActive == HIGH || Beet_4_WaessernActive == HIGH || 
       Rasen_1_WaessernActive == HIGH || Rasen_2_WaessernActive == HIGH || 
       TopfPflanzen_1_WaessernActive == HIGH || TopfPflanzen_2_WaessernActive == HIGH || Wasserstelle_EinActive == HIGH)      
       {
      digitalWrite(GPIO_23, LOW); //Pumpe einschalten (Low aktiv)
      Bewaesserungspumpe = HIGH;
      }
      

  if (((Beet_1_WaessernActive == LOW) && (Beet_2_WaessernActive == LOW) && (Beet_3_WaessernActive == LOW) && (Beet_4_WaessernActive == LOW) &&
      (Rasen_1_WaessernActive == LOW) && (Rasen_2_WaessernActive == LOW) && (TopfPflanzen_1_WaessernActive == LOW) && (TopfPflanzen_2_WaessernActive == LOW) && 
      (Wasserstelle_EinActive == LOW)) || ((ManualOperation_Active == LOW) && (AutoOperation_Prepare == LOW)))
      {
        digitalWrite(GPIO_23, HIGH); // Pumpe ausschalten
        Bewaesserungspumpe = LOW;
      }


    if (ManualOperation_Active == HIGH || AutoOperation_Active == HIGH || Wasserstelle_EinActive == HIGH)
      { 
        digitalWrite(GPIO_25, LOW); //Pumpe einschalten
        Trafo_24VAC = HIGH;
      }
     
     if (ManualOperation_Active == LOW && AutoOperation_Active == LOW && Wasserstelle_EinActive == LOW)
      { 
        digitalWrite(GPIO_25, HIGH); //Pumpe ausschalten
        Trafo_24VAC = LOW;
      }

/**********************************************************************************************************************************************
 *  Anlage für den Winterbetrieb Entwässern
 * ********************************************************************************************************************************************/
    // Entwässerungsbetrieb einschalten
    if (AnlageEntwaessern_Active == HIGH && AutoOperation_Active == LOW && AutoOperation_Prepare == LOW && ManualOperation_Active == LOW && Wasserstelle_EinActive == LOW)
        {
          digitalWrite(GPIO_2, LOW); // Ventil Beet 1 öffnen
          digitalWrite(GPIO_4, LOW); // Ventil Beet 2 öffnen
          digitalWrite(GPIO_5, LOW); // Ventil Beet 3 öffnen
          digitalWrite(GPIO_18, LOW); // Ventil Beet 4 öffnen
          digitalWrite(GPIO_19, LOW); // Ventil Rasen 1 öffnen
          digitalWrite(GPIO_21, LOW); // Ventil Rasen 2 öffnen
          digitalWrite(GPIO_17, LOW); // Ventil Töpfe 1 öffnen
          digitalWrite(GPIO_16, LOW); // Ventil Töpfe 2 öffnen
          digitalWrite(GPIO_22, LOW); // Ventil Wasserstellen öffnen
          digitalWrite(GPIO_25, LOW); // Ventil Trafo für Ventilspannung einschalten
        }

}

