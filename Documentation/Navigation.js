

//-----------------------------------------------------------------------------
function GenerateNavigation(name, file)
{
	IntroductionNavGroup();
	DownloadGroup();
	CompileNavGroup();
	HowToNavGroup();
	DocumentationNavGroup(); 
	
	MiscNavGroup();
	if (name != 'Home') {
		document.getElementById('breadcrumb').innerHTML=GenerateBreadCrumb(name, file);
		document.getElementById('slogan').innerHTML=name;
	}
}

//-----------------------------------------------------------------------------
function GenerateBreadCrumb(name, file)
{
	var html = "<a href=\"index.html\">Home</a> &#8250; <a href=\"" + file + "\">" + name + "</a>";	
	return html;
}

//-----------------------------------------------------------------------------
function DownloadGroup()
{
	var html = "<h2>Downloads</h2><ul>";
	var pages = new Array();
	pages.push("Source Codes", "SourceCodes.html");
	pages.push("Pre-Compiled Binaries", "Binaries.html");
	pages.push("Sample Data", "SampleData.html");
	for(i = 0; i < pages.length; i=i+2)
	{
		html = html + "<li><a href=\"" + pages[i+1] + "\">" + pages[i] + "</a></li>";
	}
	html += "</ul>";
	document.write(html);
}

//-----------------------------------------------------------------------------
function IntroductionNavGroup()
{
	var html = "<h2>Introduction</h2><ul>";
	var pages = new Array();
	pages.push("Home", "index.html");
	pages.push("Background", "Background.html");
	pages.push("MXA Specification", "MXASpecification.html");
	for(i = 0; i < pages.length; i=i+2)
	{
		html = html + "<li><a href=\"" + pages[i+1] + "\">" + pages[i] + "</a></li>";
	}
	html += "</ul>";
	document.write(html);
}

//-----------------------------------------------------------------------------
function CompileNavGroup()
{
	var html = "<h2>Compiling</h2><ul>";
	var pages = new Array();
	pages.push("System Requirements", "SystemRequirements.html");
	pages.push("Compile Notes", "CompileNotes.html");
	for(i = 0; i < pages.length; i=i+2)
	{
		html = html + "<li><a href=\"" + pages[i+1] + "\">" + pages[i] + "</a></li>";
	}
	html += "</ul>";
	document.write(html);
}

//-----------------------------------------------------------------------------
function HowToNavGroup()
{
	var html = "<h2>How To</h2><ul>";
	var pages = new Array();
	pages.push("Including MXA in your Project", "UsingMXAInOtherProjects.html");
	pages.push("How To Write a Data Import Delegate", "HowToWriteDataImportDelegate.html");
	pages.push("Importing Data into HDF5 using MXA APIs", "ImportingData.html");
	for(i = 0; i < pages.length; i=i+2)
	{
		html = html + "<li><a href=\"" + pages[i+1] + "\">" + pages[i] + "</a></li>";
	}
	html += "</ul>";
	document.write(html);
}


//-----------------------------------------------------------------------------
function DocumentationNavGroup()
{
	var html = "<h2>Documentation</h2><ul>";
	var pages = new Array();
	pages.push("XML Usage", "XML_Usage.html");
	pages.push("Examples", "Examples.html");
	pages.push("Doxygen API", "API-Docs/html/classes.html");
	for(i = 0; i < pages.length; i=i+2)
	{
		html = html + "<li><a href=\"" + pages[i+1] + "\">" + pages[i] + "</a></li>";
	}
	html += "</ul>";
	document.write(html);
}

//-----------------------------------------------------------------------------
function MiscNavGroup()
{
	var html = "<h2>Misc</h2><ul>";
	var pages = new Array();
	pages.push("Todo List", "Todo.html");
	pages.push("MXA Utilities", "MXAUtilities.html");
	for(i = 0; i < pages.length; i=i+2)
	{
		html = html + "<li><a href=\"" + pages[i+1] + "\">" + pages[i] + "</a></li>";
	}
	html += "</ul>";
	document.write(html);
}


//-----------------------------------------------------------------------------
function navigationCells(activePage)
{

var innerHTML = ""; // = "<div class=\"nav_inactive\"><a href=\"" + pageFiles[0] + "\">" + pageNames[0] + "</a></div>";
var i =0;
var divclass = "nav_inactive";
for (i = 0; i < arraySize; i=i+1)
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

//-----------------------------------------------------------------------------
function generateFooter()
{
	document.write("<span class=\"left\"> &copy; 2009 &nbsp;");
	document.write("<a href=\"index.html\">MXADataModel </a>. Valid ");
	document.write("<a href=\"http://jigsaw.w3.org/css-validator/check/referer\">CSS</a> &amp; ");
	document.write("<a href=\"http://validator.w3.org/check?uri=referer\">XHTML</a> </span> ");
	document.write(" <span class=\"right\">Design by <a href=\"http://arcsin.se/\">Arcsin</a> ");
	document.write(" <a href=\"http://templates.arcsin.se/\">Web Templates</a>");
	document.write( "</span>");
	document.write("<div class=\"clearer\"></div>");
}

// -----------------------------------------------------------------------------
function NavList(activePage)
{
  var innerHTML = "<ul>";

  for (i = 0; i < arraySize; i=i+1)
  {
    if (pageNames[i] == activePage)
    {
      divclass = "nav_active";
    } 
    else 
    {
      divclass = "nav_inactive";
    }
    innerHTML = innerHTML + "<li><a href=\"" + pageFiles[i] + "\">" + pageNames[i] + "</a></li>";
  }
  innerHTML = innerHTML + "</ul>";
  document.write(innerHTML);
}

