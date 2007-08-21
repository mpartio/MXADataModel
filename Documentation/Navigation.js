var pageNames = new Array()
var pageFiles = new Array()
pageNames[0] = "Home"
pageFiles[0] = "index.html"
pageNames[1] = "Background"
pageFiles[1] = "Background.html"
pageNames[2] = "Todo List"
pageFiles[2] = "Todo.html"
pageNames[3] = "System Requirements"
pageFiles[3] = "SystemRequirements.html"
pageNames[4] = "Comple Notes (XP)"
pageFiles[4] = "CompileNotesXP.html"
pageNames[5] = "Compile Notes (Unix)"
pageFiles[5] = "CompileNotesUnix.html"
pageNames[6] = "Including MXA in your Project"
pageFiles[6] = "UsingMXAInOtherProjects.html"
pageNames[7] = "XML Usage"
pageFiles[7] = "XML_Usage.html"
pageNames[8] = "Doxygen API"
pageFiles[8] = "../API-Docs/html/index.html"
pageNames[9] = "Examples"
pageFiles[9] = "Examples.html"

function navigationCells(activePage)
{

var innerHTML = ""; // = "<div class=\"nav_inactive\"><a href=\"" + pageFiles[0] + "\">" + pageNames[0] + "</a></div>";
var i =0;
var divclass = "nav_inactive";
for (i = 0; i < 10; i=i+1)
{
  if (pageNames[i] == activePage)
  {
    divclass = "nav_active";
  } 
  else 
  {
    divclass = "nav_inactive";
  }
  innerHTML = innerHTML + "<div class=\"" + divclass + "\"><a href=\"" + pageFiles[i] + "\">" + pageNames[i] + "</a></div>";
}
return innerHTML;
}