'Fordert Eingabe eines Programmnamens
'Prueft Eingabe auf Richtigkeit
'Aufruf: for /f "tokens=*" %i in ('cscript //nologo programname.vbs') do set programname=%i

set objRgx = CreateObject("vbScript.RegExp")
objRgx.Pattern = "[^a-zA-Z_0-9]"

pname=""
Do
  pname=inputbox("Gebe bitte Namen des Programms an!","Programmname",pname)

  set objRegMatches = objRgx.Execute(pname)
  if objRegMatches.Count = 0 then
    Exit Do
  end if

  MsgBox "Ungültiger Name"&chr(10)&"Name darf nur A-Z, a-z, 0-9 und _ enthalten", vbOKOnly+vbExclamation
Loop

wscript.echo pname
