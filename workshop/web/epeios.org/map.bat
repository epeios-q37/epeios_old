sabcmd file://t:/map.xsl site.xml i/map.xml
reveal t:/windows.crp site.crp i/site.crp
reveal i/site.crp site.xml i/site.xml
sabcmd file://t:/epeios.org/page.xsl i/site.xml /website//map.html "$hierarchy=" "$name=map"
start \website\map.html