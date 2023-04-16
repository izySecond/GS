# Workflow für die Anpassung der STM32CubeF4 Firmware
Der erste Abschnitt stellt einige umfassende Zusammenhänge kurz dar. Weitere Informationen stehen in der Dokumentation des Repos tiboard_dev/ITS_BRD_LIB.
## Relevante Repos
Die SW des ITSboards und dessen Variante ITSboardAtHome wird auf dem GitLab Server der HAW in vier Repos gehalten.
- **tiboard_dev/ITS_BRD**: Dieses Repo speichert ein oder mehrere embedded Programme für das ITSboard oder dessen Variante ITSboardAtHome. So kann zum Beispiel ein Studierender alle Aufgaben des GS Praktikums in einem oder mehreren Repos dieser Art speichern.
- **tiboard_dev/ITS_BRD_LIB**: Dieses Repo speichert die HAW SW für das ITSboard. Typische Beispiele sind der erweiterte StartUp Code oder das KeyPad.
- **tiboard_dev/STM32CubeF4** Dieses Repo beinhaltet direkte Anpassungen der STM STM32CubeF4 Firmware an die SW des ITSboards. Funktionen, die keine direkte Anpassung der STM Firmware sind, werden im Repo tiboard_dev/ITS_BRD_LIB gespeichert. 
- **tiboard_dev/DisplayWareshare** Dieses Repo speichert die Software zum 4inch_TFT_Touch_Shield von Wareshare und deren Anpassung an die STM32CubeF4. Diese Software wurde als tar geliefert.
- **tiboard_dev/ITS_BRD_TOOLS**: Dieses Repo speichert Tools für die Software des ITSboards. Dazu zählt zum Beispiel das Tool zur Generierung großer Fonts oder das Tool zur Betankung des ITSBoards mit Fonts.

Weiterhin werden folgende GitHub Repos verwendet:
- **STMicroelectronics/STM32CubeF4** Dieses ist das Original-Repo von  STM, das den aktuellen Stand der STM32CubeF4 Firmware beinhaltet.
- **FranzKorf/STM32CubeF4** Das Repo ist ein Fork des Original-Repos für den User FranzKorf. Über diesen Weg können Pull Requests an das Original Repo STMicroelectronics/STM32CubeF4 von STM geschickt werden.

### Abhängigkeiten zwischen den Repos
Die Repos der Software des ITSboards sind über Submodule wie folgt verbunden.
- tiboard_dev/ITS_BRD enthält das Submodule tiboard_dev/ITS_BRD_LIB
- tiboard_dev/ITS_BRD_LIB enthält die Submodule tiboard_dev/STM32CubeF4 und tiboard_dev/DisplayWareshare

Über das GitHub Repo FranzKorf/STM32CubeF4 wird die Relation zwischen dem Repo tiboard_dev/STM32CubeF4 und dem GitHub Repo STMicroelectronics/STM32CubeF4 gehalten, so dass Pull Requests möglich sind.

Das Repo tiboard_dev/ITS_BRD_TOOLS hat eine indirekte Relation zum Repo tiboard_dev/ITS_BRD_LIB, da einige Tools C Code erzeugen, der im Repo tiboard_dev/ITS_BRD_LIB eingebunden ist.

### Abhängigkeiten zwischen den STM32CubeF4 Repos
Die STM32CubeF4 Firmware wird in den Repos 
- tiboard_dev/STM32CubeF4 auf dem GitLab Server der HAW
- FranzKorf/STM32CubeF4 auf GitHub
- STMicroelectronics/STM32CubeF4 auf GitHub
verwaltet.

## Workflow für die STM32CubeF4 Software
### Anforderungen
Folgende Tätigkeiten muss der Workflow unterstützen:
- STM entwickelt die STM32CubeF4 FW weiter. Somit müssen Änderungen aus dem Original-Repo STMicroelectronics/STM32CubeF4 über den Fork FranzKorf/STM32CubeF4 in das Repo tiboard_dev/STM32CubeF4 übertragen werden.
- Andererseits kann das HAW Team Bugs in der STM32CubeF4 FW fixen bzw. Änderungen in der STM32CubeF4 FW vornehmen, die in das Original-Repo gespielt werden sollen. Diese Änderungen werden in das GitHub FranzKorf/STM32CubeF4 gespielt und über einen Pull Request an das Original-Repo STMicroelectronics/STM32CubeF4 weitergegeben.
- Das HAW Team wird Anpassungen in der STM32CubeF4 FW vornehmen, die nicht in die Original-Repo zurückgespielt werden sollen. SW Komponenten, die nicht direkt mit der STM Software verzahnt sind, werden nicht in diesem Repo sondern im Repo ITS_BRD_LIB gespeichert.
- Feature Branches für Bug Fixes und Anpassungen.

### Workflow
Änderungen werden im Repo tiboard_dev/STM32CubeF4 des GitLab Servers der HAW gepflegt. Über das GitHub Repo FranzKorf/STM32CubeF4 wird die Verbindung zum Original-Repo von STM gehalten.

#### Verwendete Branches
- STM: Dieser Branch enthält den aktuelle FW Stand  des Original-Repo von  STM.
- Master: Dieser Branch enthält die STM FW mit den Anpassungen für die SW des ITS Boards.
- Feature Branches für STM Fixes und Erweiterungen, die in das Original-Repo von STM zurück gespielt werden sollen.
- Feature Branches für Anpassungen der STM SW, die für die HAW SW Entwicklung notwendig sind und sich direkt auf die STM SW beziehen. 

#### Arbeitsabläufe im Workflow
Folgende typischen Arbeitsabläufe müssen unterstützt werden:
- **Erstellung eines Bugfix oder einer Anpassung, die in das Original-Repo von STM zurückfließen soll**
	Es wird ein Feature Branch, der vom STM Branch abzweigt, erzeugt und schließlich wieder in den STM Branch gemerged.
- **Erstellung einer Anpassung der STM Firmware an die ITSboard Software, die sich nicht direkt auf die STM FW bezieht**. 
	Es wird ein Feature Branch, der vom master Branch abzweigt, erzeugt und schließlich wieder in den master Branch gemerged.
- **Merge des STM Branch in der Master Branch**
	Nach einen Bugfix in STM Branch wird dieser in den Master Branch gemerged werden.
- **Integration eines neuen SW Stands aus dem Original-Repo von STM**
	Auf GitHub wird der Update des Master des Original-Repo STMicroelectronics/STM32CubeF4 in dessen Fork FranzKorf/STM32CubeF4 gespielt. Anschießend wird über zwei Remote Repos der Master Branch des GitHub Repo FranzKorf/STM32CubeF4 in den STM Branch des tiboard_dev/STM32CubeF4 Repos gemerged. Abschließend wird im tiboard_dev/STM32CubeF4 Repo der STM Branch in den Master Branch gemerged.
- **Erstellung eines Pull Requests an das Original-Repo von STM**
	Über zwei Remote Repos wird der STM Branch in das GitHub Repo FranzKorf/STM32CubeF4 gemerged. Abschließend wird auf GitHub ein Pull Request an das Original-Repo von STM gestellt.
	
## Einsatz von zwei Remote Repos zur Kopplung des GitHub Repos FranzKorf/STM32CubeF4 mit den HAW GitLab Repo tiboard_dev/STM32CubeF4
Da das Original-Repo der STM32CubeF4 FW auf GitHub liegt und die Anpassungen der STM32CubeF4 FW an die Software des ITSboards im tiboard_dev/STM32CubeF4 Repo des GitLab Servers der HAW gespeichert sind, wird die Verbindung zwischen diesen Repos über ein lokales Repo mit zwei remote Repos realisiert.

### Verbindung zum Originalrepo von STM32
Zuerst wird das tiboard_dev/stm32cubef4 in ein lokales Repo auf dem Arbeitsrechner geclont. Anschließend wird das remote Repo umbenannt. Wichtig ist, dass in den STM branch gewechselt wird.
- git clone https://git.haw-hamburg.de/tiboard_dev/stm32cubef4.git
- git remote rename origin gitlab_HAW
- git checkout STM
- git branch -r
- git branch -vv
- git status 
Nun wird das zweite remote Repo angelegt.
- git remote add github  https://github.com/FranzKorf/STM32CubeF4.git
- git fetch github
- git remote show

Nun kann das lokale Repo mit beiden remote Repos synchronisiert werden. **Wichtig ist, dass stets der Branch STM des tiboard_dev/stm32cubef4 Repos verwendet wird**.

#### Typische Arbeitsschritte
- Vergleich des lokalen Repo mit einen der beiden Remote Repos. Dazu wird der Tracking Branch umgestellt.
	- git branch -u github/master
	- git status
	oder
	- git branch -u gitlab_HAW/STM
	- git status
- Push und pull ins Repo tiboard_dev/stm32cubef4 auf dem HAW GitLab Server
	- git branch -u gitlab_HAW/STM
	- git pull oder git pull gitlab_HAW STM
	- git push oder git push gitlab_HAW STM
- Push und pull ins Repo FranzKorf/STM32CubeF4 auf GitHub.
	- git branch -u github/master
	- git pull oder git pull github master
	- git push oder git push github master
 