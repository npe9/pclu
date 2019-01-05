;; .emacs entries for CLU mode

(defvar clu-home (getenv "CLUHOME"))

(setq load-path 
      (append (list (concat clu-home "/emacs"))
	      load-path))

(setq auto-mode-alist
      (append '(("\\.clu$" . clu-mode)
		("\\.spc$" . clu-mode))
	      auto-mode-alist))

(autoload 'clu-mode "clu.elc" "CLU mode" t nil)

(setq clu-doc-file-name (concat clu-home "/emacs/CLU-DOC"))
