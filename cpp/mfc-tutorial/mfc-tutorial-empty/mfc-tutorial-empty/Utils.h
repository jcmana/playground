#pragma once

#include <objbase.h>
#include <propkey.h>
#include <propsys.h>
#include <propvarutil.h>
#include <shobjidl.h>
#include <wrl/client.h>

BOOL CreateShortcut(const WCHAR * shortcutPath, const WCHAR * appUserModelID, const WCHAR * iconFilePath, int iconIndex);
