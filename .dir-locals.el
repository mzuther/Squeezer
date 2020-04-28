(
 (nil . ((eval . (let* ((relative-root "")
                        (root (concat (projectile-project-root) relative-root))
                        (language-standard "c++14")
                        (include-path (list (concat root "JuceLibraryCode/")
                                            (concat root "libraries/juce/modules/")
                                            (concat root "libraries/googletest/googletest/include/")
                                            (concat root "Source/frut/")
                                            (concat root "libraries/"))))

                   (setq-local flycheck-clang-language-standard language-standard)
                   (setq-local flycheck-clang-include-path include-path)

                   (setq-local flycheck-gcc-language-standard language-standard)
                   (setq-local flycheck-gcc-include-path include-path)))))
 (c-mode . ((mode . c++)))
 )
