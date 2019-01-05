;;;
;;; This file sets up the emacs environment to start-up CLU mode
;;; when editing .clu and .equ files.
;;; You should add a line to ~/.emacs that loads in this file.
;;;
;;;	(load-file ".../setup.el")

;;;
;;; Change the value of the following variable to match the location
;;; of the CLU emacs-lisp directory (usually $CLUHOME/elisp)

(defvar clu-lisp-directory "/usr/pclu/elisp"
  "CLU lisp directory location")

;;;
;;; Use clu-mode for .clu and .equ files

(setq auto-mode-alist
      (append (list (cons "\\.clu$"	'clu-mode)
		    (cons "\\.equ$"	'clu-mode)
		    )
	      auto-mode-alist
	      )
      )
;;;
;;; Set-up the load-path to include the clu elisp files

(setq load-path (cons clu-lisp-directory load-path))

;;;
;;; Setup CLU documentation file pointer

(setq clu-doc-file-name (concat clu-lisp-directory "/CLU-DOC"))

;;;
;;; Auto-load clu elisp files

(autoload 'clu-mode "clu" "Clu mode" t)
