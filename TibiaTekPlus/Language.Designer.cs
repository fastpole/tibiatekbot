﻿//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//     Runtime version:2.0.50727.1433
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------

namespace TibiaTekPlus {
    using System;
    
    
    /// <summary>
    ///   A strongly-typed resource class, for looking up localized strings, etc.
    /// </summary>
    // This class was auto-generated by the StronglyTypedResourceBuilder
    // class via a tool like ResGen or Visual Studio.
    // To add or remove a member, edit your .ResX file then rerun ResGen
    // with the /str option, or rebuild your VS project.
    [global::System.CodeDom.Compiler.GeneratedCodeAttribute("System.Resources.Tools.StronglyTypedResourceBuilder", "2.0.0.0")]
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
    [global::System.Runtime.CompilerServices.CompilerGeneratedAttribute()]
    public class Language {
        
        private static global::System.Resources.ResourceManager resourceMan;
        
        private static global::System.Globalization.CultureInfo resourceCulture;
        
        [global::System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1811:AvoidUncalledPrivateCode")]
        internal Language() {
        }
        
        /// <summary>
        ///   Returns the cached ResourceManager instance used by this class.
        /// </summary>
        [global::System.ComponentModel.EditorBrowsableAttribute(global::System.ComponentModel.EditorBrowsableState.Advanced)]
        public static global::System.Resources.ResourceManager ResourceManager {
            get {
                if (object.ReferenceEquals(resourceMan, null)) {
                    global::System.Resources.ResourceManager temp = new global::System.Resources.ResourceManager("TibiaTekPlus.Language", typeof(Language).Assembly);
                    resourceMan = temp;
                }
                return resourceMan;
            }
        }
        
        /// <summary>
        ///   Overrides the current thread's CurrentUICulture property for all
        ///   resource lookups using this strongly typed resource class.
        /// </summary>
        [global::System.ComponentModel.EditorBrowsableAttribute(global::System.ComponentModel.EditorBrowsableState.Advanced)]
        public static global::System.Globalization.CultureInfo Culture {
            get {
                return resourceCulture;
            }
            set {
                resourceCulture = value;
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to Error.
        /// </summary>
        public static string Error {
            get {
                return ResourceManager.GetString("Error", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to Information.
        /// </summary>
        public static string Information {
            get {
                return ResourceManager.GetString("Information", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to Unable to load the following plug-in:
        ///Title: {0}.
        ///Author: {1}.
        ///Reason: The file &apos;{2}.dll&apos; was not found..
        /// </summary>
        public static string kernel_error1 {
            get {
                return ResourceManager.GetString("kernel_error1", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to Unable to load the following plug-in:
        ///{0}.dll..
        /// </summary>
        public static string kernel_error2 {
            get {
                return ResourceManager.GetString("kernel_error2", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to Are you sure that you want to close TibiaTek Plus?.
        /// </summary>
        public static string mainForm_FormClosing {
            get {
                return ResourceManager.GetString("mainForm_FormClosing", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to Are you sure that you want to use the following skin: {0}?
        ///Warning: You will be required to restart this application..
        /// </summary>
        public static string mainForm_skinSelected {
            get {
                return ResourceManager.GetString("mainForm_skinSelected", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to Collapse this category..
        /// </summary>
        public static string menuWebBrowser_Collapse {
            get {
                return ResourceManager.GetString("menuWebBrowser_Collapse", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to Expand this category..
        /// </summary>
        public static string menuWebBrowser_Expand {
            get {
                return ResourceManager.GetString("menuWebBrowser_Expand", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to There are no available skins. Please first, uninstall TibiaTek Plus and then install it again..
        /// </summary>
        public static string Program_NoSkins {
            get {
                return ResourceManager.GetString("Program_NoSkins", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to Question.
        /// </summary>
        public static string Question {
            get {
                return ResourceManager.GetString("Question", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to The skin &apos;{0}&apos; is missing the file:
        ///{1}
        ///Please re-install this skin..
        /// </summary>
        public static string Skin_MissingSkinFile {
            get {
                return ResourceManager.GetString("Skin_MissingSkinFile", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to Ready..
        /// </summary>
        public static string statusBarLabelText_Ready {
            get {
                return ResourceManager.GetString("statusBarLabelText_Ready", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to Warning.
        /// </summary>
        public static string Warning {
            get {
                return ResourceManager.GetString("Warning", resourceCulture);
            }
        }
    }
}
