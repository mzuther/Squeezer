;;  ----------------------------------------------------------------------------
;;  
;;  FrutJUCE
;;  ========
;;  Common classes for use with the JUCE library
;;  
;;  Copyright (c) 2010-2021 Martin Zuther (http://www.mzuther.de/)
;;  
;;  This program is free software: you can redistribute it and/or modify
;;  it under the terms of the GNU General Public License as published by
;;  the Free Software Foundation, either version 3 of the License, or
;;  (at your option) any later version.
;;  
;;  This program is distributed in the hope that it will be useful,
;;  but WITHOUT ANY WARRANTY; without even the implied warranty of
;;  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;;  GNU General Public License for more details.
;;  
;;  You should have received a copy of the GNU General Public License
;;  along with this program.  If not, see <http://www.gnu.org/licenses/>.
;;  
;;  Thank you for using free software!
;;  
;;  ----------------------------------------------------------------------------


;;  ############################################################################
;;
;;  WARNING: this file was auto-generated, please do not edit!
;;
;;  ############################################################################

(
 (nil . ((eval . (let* ((relative-root "../../")
                        (root (concat (projectile-project-root) relative-root))
                        (language-standard "c++17")
                        (include-path (list
                                       (concat root "JuceLibraryCode/")
                                       (concat root "libraries/")
                                       (concat root "libraries/juce/modules/")
                                       (concat root "Source/frut/")
                                       (concat root "libraries/googletest/googletest/include/")
                                       ))
                        (includes (list
                                   (concat root "Source/frut/FrutHeader.h")
                                   )))

                   (setq-local flycheck-clang-language-standard language-standard)
                   (setq-local flycheck-clang-include-path include-path)
                   (setq-local flycheck-clang-includes includes)
                   (setq-local flycheck-clang-warnings `(,@flycheck-clang-warnings
                                                         "no-pragma-once-outside-header"))

                   (setq-local flycheck-gcc-language-standard language-standard)
                   (setq-local flycheck-gcc-include-path include-path)
                   (setq-local flycheck-gcc-includes includes)))))
 (c-mode . ((mode . c++)
            ( c-basic-offset . 3)))
 (c++-mode (c-basic-offset . 3))
 )
