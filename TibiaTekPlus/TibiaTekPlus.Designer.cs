﻿//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//     Runtime Version:2.0.50727.1433
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------

namespace TibiaTekPlus {
    
    
    [global::System.Runtime.CompilerServices.CompilerGeneratedAttribute()]
    [global::System.CodeDom.Compiler.GeneratedCodeAttribute("Microsoft.VisualStudio.Editors.SettingsDesigner.SettingsSingleFileGenerator", "9.0.0.0")]
    public sealed partial class TibiaTekPlus : global::System.Configuration.ApplicationSettingsBase {
        
        private static TibiaTekPlus defaultInstance = ((TibiaTekPlus)(global::System.Configuration.ApplicationSettingsBase.Synchronized(new TibiaTekPlus())));
        
        public static TibiaTekPlus Default {
            get {
                return defaultInstance;
            }
        }
        
        [global::System.Configuration.UserScopedSettingAttribute()]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [global::System.Configuration.DefaultSettingValueAttribute("Default")]
        public string Skin {
            get {
                return ((string)(this["Skin"]));
            }
            set {
                this["Skin"] = value;
            }
        }
    }
}
