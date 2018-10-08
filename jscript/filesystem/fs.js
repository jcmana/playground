var fileSystem = new ActiveXObject("Scripting.FileSystemObject");
var fileSystemFolder = fileSystem.GetFolder("F:\\git\\avg_common\\avg\\automation\\");
var fileList = new Enumerator(fileSystemFolder.files);

fileList.end();

WScript.Echo("kokot");

