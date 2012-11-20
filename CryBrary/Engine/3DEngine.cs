﻿using System.Runtime.CompilerServices;
using CryEngine.Native;

namespace CryEngine
{
    /// <summary>
    /// Provides an direct interface to the 3DEngine.
    /// </summary>
    public static class Engine
    {
        // TODO: Find a better place for this or fill up Engine with other stuff. It's so lonely!
        public static void ActivatePortal(Vec3 pos, bool activate, string entityName)
        {
            Native3DEngineMethods.Instance.ActivatePortal(pos, activate, entityName);
        }
    }
}
