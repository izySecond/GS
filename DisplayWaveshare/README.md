# Allgemeines
- Dieses Repo speichert die Software zum 4inch TFT Touch Shield Display von Wareshare.
- Das Repo kann momentan separat nicht übersetzt werden. Es muss in ein ITS_BRD Repo aus submodule eingebunden sein.

# Dokumentation
- **Wiki** Unter der Seite https://www.waveshare.com/wiki/4inch_TFT_Touch_Shield gibt es ein kleines Wiki zu dem Board.
- **Datenblätter etc** Im Ordner Doc liegt Dokumentation zu dem Board. Sie stammt aus dem Wiki.
- **Interfaces** Weiterhin werden die Anpassungen für das Nucleo Board F429ZI sowie die Interfaces zum Board und zu den Anwendungen beschrieben.

# Software
- Die Basis Software stammt aus dem zip File https://www.waveshare.com/wiki/File:4inch_TFT_Touch_Shield_Code.7z, das auch ein Beispiel für den STM32 F103RB enthält.
- In diesem Repo wird nur der Code zur Ansteuerung des Displays verwalten. Der restliche Code, wie z.B: ein Beispiel für Arduino oder den STM32 F103RB steht nur im Zip File. Es wird im Ordner Zip archiviert.

# Schnittstellen
- DEV_config.c spannt die Schnittstelle spannt die Schnittstelle zur HW auf.
- LCD_GUI.c bildet die Schnittstelle des LCD zur Anwendung.
- LCD_Touch.c bildet die Schnittstelle des Touch Interface zur Anwendung

# Resource
Folgende Ressourcen werden verwendet
- PINs : s. Schematics
- TIM4 zum Betreiben des Displays
- SPI1 : Der Baustein zum Betreiben des LCD und der Baustein zum Betreiben des Touch werden über SPI1 mit unterschiedlichen Geschwindigkeiten betrieben. Je nach Prozessor gibt es kleine Unterschiede bei der Umstellung der Geschwindigkeit.




