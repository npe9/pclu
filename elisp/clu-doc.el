;;; -*- Mode: Emacs-Lisp -*-

;;; File:		clu-doc.el
;;; Description:	Use emacs-style help to view the CLU manual
;;; Author:		Boaz Ben-Zvi  <boaz@lcs.mit.edu>
;;; Created:		January 30, 1991
;;; Version:		1.1, 2-mar-91

;; This file provides facilities to read the CLU Manual in a fashion similar
;; to reading Emacs' help.  CLU is a programming language that was developed
;; in MIT (see the book "Abstraction and Specification in Program Developement"
;; by Barbara Liskov and John Guttag, The MIT Press, 1986).

;;;;;    INSTALLATION
;; To use, you should have the CLU-DOC file installed; this files contains
;; the CLU manual in a format similar to Emacs' DOC file. 
;;   You can set the variable  clu-doc-file-name  (in your .emacs or in this
;; file) to the full path name of your CLU-DOC file, else you will be 
;; prompted for that name (can also be set by calling  M-x clu-doc-visit-file).
;;   The elisp file  CLU-DOC-keys.el  (in the same directory as the CLU-DOC 
;; file) contains keys and indexes for the CLU-DOC file; you can generate 
;; the CLU-DOC-keys.el yourself by typing   M-x  clu-doc-snarf-file .

;;;;;;    USE
;; To get CLU help (after loading this file), type <help-key> (typically C-h)
;; followed by one of:
;;   d - for documentation on a CLU type or operation (e.g., REAL, stream$copy)
;;   g - for a CLU general topic (e.g., CLUSTERS, input/output, SYNTAX)
;;   / - for CLU apropos, all entries matching some string (e.g., file)
;; note: case is unimpotrtant, use TAB for completion, word under cursor is
;;       used as a guess (if such entry exists).

;;;;;;    ADDING USER DEFINED  ABSTRACTIONS
;; You can add your user defined abstractions (i.e. procedures, iterators,
;; and clusters+operations ) to this on-line manual facility.  Calling  
;; M-x clu-doc-add-file would prompt you for a Clu file name to add to the 
;; current Clu manual.  Once added, you can apply the regular CLU help 
;; operations above to the abstractions defined in your file.
;;    For each user defined abstraction, this program extracts the definition 
;; and all the following comments up to the beginning of the code itself.
;;    To add all the CLU files in some directory use M-x clu-doc-add-directory.
;; Note that the CLU files are identified by the suffix kept in the variable
;; clu-doc-file-suffix (normally ".clu"), if you want to load all the files 
;; with different suffix (e.g., ".spc") you should modify this variable.
;;     LIFETIME: Once added, your definitions remain in the current Clu manual
;; for the rest of the Emacs session. There is no way to "erase" or "replace"
;; definitions. Your definitions disappear when you exit emacs unless you 
;; dump them.
;;     DUMPING: To dump only your definitions use M-x clu-doc-dump-file. Your
;; definitions would be APPENDED to the dump file, so you can dump several
;; Clu files into a single dump file. To load the dump file (i.e., add your 
;; dumped definitions to the current Clu manual) use  M-x clu-doc-load-file 
;; or set the variable  clu-doc-dumped-files-to-load  to a list of names
;; of dump files to be loaded with the CLU-DOC file.
;;     TOTAL DUMPING: M-x clu-doc-total-dump  dumps the whole current CLU 
;; manual (including any added user definitions) into a new CLU-DOC file 
;; (and makes a new -keys.el file); the new CLU-DOC file can be used instead 
;; of the original CLU-DOC.

;;;
;;;  FORMAT
;;;
;; The format of the CLU-DOC file is a sequence of entries, each begining with
;; clu-doc-start string followed by a two letter code:
;;    GD : general CLU topic documentation
;;    BT/BP/BI : built-in type/procedure/iterator
;;    OB : operation on built-in type
;;    UT/UP/UI : user-defined type/procedure/iterator
;;    OU : operation on User Defined type
;; The code is followed (no spaces etc.) by the name of the entry, CR and the
;; body of the documentation. ( above:  type == cluster )

;;;
;;;  Variables
;;;

(defvar clu-doc-version "1.1" "The current version of clu-doc")

(defvar clu-doc-start "" "The special string that starts each doc\
 record in the CLU-DOC file")

(defvar clu-doc-key-length 2 "Length of the description code following \
the clu-doc-start string in the CLU-DOC file")

(defvar clu-doc-edit nil "*If non nil, instead of just printing out a copy \
of the documentation in the other window, we actually visit the CLU-DOC file. \
This is useful for editing that file.")

(defvar clu-doc-buffer nil "buffer where the CLU documentation lives")

(defvar clu-doc-file-name nil
"*File name of the current CLU-DOC file, if null user is prompted for it.")
;; Usually CLU-DOC should be the name used and CLU-DOC-keys.el will hold 
;; the keys to that file.

(defvar clu-doc-file-suffix ".clu" "*Suffix of Clu files")

(defvar clu-doc-dumped-files-to-load nil 
    "*List of clu doc files (each created by clu-doc-dump-file) that is loaded
at initialization time into the current session of the CLU help facility")

;;;
;;;  Key Binding in the help-map
;;;
(defvar clu-doc-symbol-key "d" "*Help key for CLU symbols")
(defvar clu-doc-general-key "g" "*Help key for CLU general topics")
(defvar clu-doc-apropos-key "/" "*Help key for CLU apropos")
(define-key help-map clu-doc-symbol-key 'clu-doc-find-symbol)
(define-key help-map clu-doc-general-key 'clu-doc-find-topic)
(define-key help-map clu-doc-apropos-key 'clu-doc-find-apropos)

(defun clu-doc-help ()
    "Print help on using the on-line CLU manual"
    (interactive)
    (let ((help-key (substitute-command-keys "\\[help-for-help]")))
	(setq help-key (substring help-key 0 (string-match " " help-key)))
	(with-output-to-temp-buffer "*Help*"
	    (princ "The CLU on-line manual\n")
	    (princ "======================\n\n")
	    (princ " To get CLU help, type  ")
	    (princ help-key)
	    (princ "  (help key) followed by one of the keys:\n")
	    (princ "\n   \'") (princ clu-doc-symbol-key) (princ "\' - ")
	    (princ "documentation on a CLU type or operation (e.g., REAL, stream$copy)")
	    (princ "\n   \'") (princ clu-doc-general-key) (princ "\' - ")
	    (princ "general CLU topic (e.g., CLUSTERS, input/output, SYNTAX)")
	    (princ "\n   \'") (princ clu-doc-apropos-key) (princ "\' - ")
	    (princ "CLU apropos, all entries matching some string (e.g. file)")
	    (princ "\n\n Note: case is unimpotrtant, use TAB for completion, word under cursor is\n")
	    (princ "given as a default (if such entry exists).\n\n")
	    (princ " To add user defined abstractions to the manual, see description of the\n")
	    (princ "following functions (type  ")
	    (princ (substitute-command-keys "\\[describe-function]"))
	    (princ " and give the function name):\n\n")
	    (princ "\tclu-doc-add-file\t  (add spec of a CLU file to current help)\n")
	    (princ "\tclu-doc-add-directory\t  (add spec of all CLU files in a directory)\n")
	    (princ "\tclu-doc-dump-file\t  (append spec of a CLU file to a dump file)\n")
	    (princ "\tclu-doc-load-file\t  (load a dump file into current help)\n")
	    (princ "\tclu-doc-total-dump\t  (create a new version of CLU-DOC)\n")
	    (princ "\nor the variables (type  ")
	    (princ (substitute-command-keys "\\[describe-variable]"))
	    (princ " and give the variable name):\n\n")
	    (princ "\tclu-doc-file-suffix\t  (recognize files as CLU files)\n")
	    (princ "\tclu-doc-dumped-files-to-load   (files loaded at initialization) \n")
	    (princ "\n Note: Adding CLU files or loading dumped files affects the current session\n")
	    (princ "of the CLU help only!  Set  clu-doc-dumped-files-to-load  in your .emacs to\n")
	    (princ "the list of dumped files you need regularly.")
	    (print-help-return-message))))

;;;
;;;   SNARF-FILE
;;;
(defun clu-doc-snarf-file (file)
  "Takes a clu-doc FILE, records the pointers into that file, and writes out 
a list of doc pointers into FILE-keys.el.  The list is suitable for the 
clu-doc-find-entry command."
  (interactive "FMake -keys.el for file: ")
  (find-file file)
  (goto-char (point-min))
  (let (temp (symbol-list nil) (general-list nil) general entry)
      (while (search-forward clu-doc-start nil t)
	  (setq temp (point))
	  (setq general (looking-at "GD"))  ;; is it a general doc. entry ?
	  (setq entry (cons (buffer-substring (+ clu-doc-key-length temp)
					      (progn (forward-sexp 1) (point)))
			    temp))
	  (if general (setq general-list (cons entry general-list))
	      (setq symbol-list (cons entry symbol-list)))
	  )
      (clu-doc-dump-keys file symbol-list general-list "snarf")
      (setq clu-symbols-doc symbol-list)
      (setq clu-general-doc general-list)))

(defun clu-doc-dump-keys (file symbol-list general-list which-one)
    "Dump the two lists in the keys.el file for the doc FILE"
    (save-excursion
	(set-buffer (generate-new-buffer "-keys.el"))
	(insert ";; This file is generated automatically by clu-doc-")
	(insert which-one "-file")
	(insert " ; it contains \n;; pointers to entries in the file  ")
	(insert file " .\n\n")
	(insert "(setq clu-symbols-doc '")
	(insert (prin1-to-string symbol-list))
	(insert ")\n\n")
	(insert "(setq clu-general-doc '")
	(insert (prin1-to-string general-list))
	(insert ")")
	(write-file (concat file "-keys.el"))))


;;;
;;;   SET DOC FILE AND BUFFER
;;;
(defun clu-doc-visit-file (file)
  (interactive (list (read-file-name "Visit clu doc table: (default CLU-DOC) "
				     default-directory
				     (concat default-directory "CLU-DOC")
				     t)))
  (setq file (expand-file-name file))
  (if (file-directory-p file)
      (setq file (concat file "CLU-DOC")))
  (if (not (file-readable-p file))
	  (error "File %s in not readable." file)
      (let ((keys-file (concat file "-keys.el")))
	  (setq clu-doc-file-name file)
	  (if (not (file-exists-p keys-file)) 
		  (error (concat "File %s does not exist. Use "
				 "clu-doc-snarf-file to generate it")
			 keys-file))
	  (if (file-readable-p keys-file)
		  (load keys-file)
	      (error "File %s in not readable." keys-file))
	  (mapcar 'clu-doc-load-file clu-doc-dumped-files-to-load)
	  (message "\t\t\t\t\t\t")
	  )))

(defun require-doc-file-in-buffer ()
    (cond ((boundp 'clu-symbols-doc) nil)  ;; already called clu-doc-visit-file
	  ((null clu-doc-file-name)
	   (clu-doc-visit-file 
	    (read-file-name 
	     "Visit clu doc table: (default CLU-DOC) "  default-directory
	     (concat default-directory "CLU-DOC") t)))
	  (t (clu-doc-visit-file clu-doc-file-name)))
    (or clu-doc-buffer    ;; already set buffer for CLU-DOC
	(save-excursion
		(set-buffer (find-file-noselect clu-doc-file-name))
		(rename-buffer "*CLU-DOC*")
		(setq clu-doc-buffer (find-file-noselect clu-doc-file-name)))))

;;;
;;;   APROPOS
;;;
(defun clu-print-match (arg)
    "Prints symbols in ARG matching clu-match-apropos (passed dynamically)"
    (cond ((string-match clu-match-apropos (upcase (car arg)))
	   (princ "   ")
	   (princ (car arg))
	   (terpri))
	  (t )))

(defun clu-doc-find-apropos (test)
    "Show all Clu symbols and topics whose names contain match for TEST"
    (interactive "sApropos CLU doc string: ")
    (require-doc-file-in-buffer)
    (with-output-to-temp-buffer "*Help*"
	(setq clu-match-apropos (upcase test))  ;;; use dynamic binding
	(princ "Matching CLU symbols found:")
	(terpri)
	(mapcar 'clu-print-match clu-symbols-doc)
	(terpri)
	(princ "Matching CLU documentation topics found:")
	(terpri)
	(mapcar 'clu-print-match clu-general-doc)
	(print-help-return-message))
    )
;;;
;;;  FIND CLU DOC
;;;
(defun clu-doc-find-symbol() 
    "Ask for a Clu symbol. Return documentation as a Clu type or operation."
    (interactive)  
    (clu-doc-find-entry 'symbols))

(defun clu-doc-find-topic() 
    "Ask for a Clu topic (e.g. TERMINAL_I/O) . Return documentation."
    (interactive)  
    (clu-doc-find-entry 'general))

(defun clu-doc-find-entry (option)
    "Based on OPTION ('symbols or 'general), ask and return Clu documentation."
    (require-doc-file-in-buffer)
    (let ((completion-ignore-case  t)
	  (till-dollar nil)
	  (clu-table (if (eq option 'symbols) clu-symbols-doc clu-general-doc))
	  (prompt (if (eq option 'general) "Describe CLU topic" 
		      "Describe CLU symbol"))
	  current-word result from-point to-point 
	  (keep-point (point))) 
	(condition-case er  ;;; try to guess word under cursor as CLU entry
		(progn
		    (if (looking-at "\\$") (forward-char 1)) ;; bug when on $
		    (forward-sexp -1)
		    (setq from-point (point))
		    (setq to-point (save-excursion (forward-sexp 1) (point)))
		    (setq current-word (buffer-substring from-point to-point))
		    ;; in case  like  array[foo]$addh , skip the [foo]
		    (cond ((looking-at "\\$")
			   (forward-sexp -1)
			   (if (re-search-backward "[^a-z_A-Z]" 1 'beg-buf)
				   (forward-char 1))
			   (setq from-point (point))
			   (re-search-forward "\\[")
			   (if (> (point) to-point) (error))
			   (setq to-point (1- (point)))
			   (setq current-word 
				 (concat (buffer-substring from-point to-point)
					 current-word)))))
	    (error))
	(goto-char keep-point) ;; return to original point
	(or (and current-word (assoc (upcase current-word) clu-table))
	    (setq current-word nil))
	(setq prompt (if current-word (concat prompt 
					      " (default " current-word "): ")
			 (concat prompt ": ")))
	(let ((symbol (completing-read prompt clu-table nil t)))
	    (if (equal symbol "") 
		    (if current-word (setq symbol current-word)  ;; default
			(error "No default for CLU symbol")))
	    (setq result (assoc symbol clu-table))
	    (or result (setq result (assoc (downcase symbol) clu-table)))
	    (or result (setq result (assoc (upcase symbol) clu-table)))
	    (or result (error "case mix up: %s not in clu-table keys" symbol)))
	(save-excursion
	    (set-buffer clu-doc-buffer)
	    (goto-char (cdr result))
	    (cond (clu-doc-edit
		   (set-fill-column 79)
		   (display-buffer clu-doc-buffer))
		  (t
		   (with-output-to-temp-buffer "*Help*"
		       (cond ((looking-at "B") (princ "Built-in "))
			     ((looking-at "U") (princ "User defined "))
			     ((looking-at "O") 
			      (setq till-dollar t)
			      (princ "Operation on "))
			     ((looking-at "GD")
			      (princ "General Documentation about: ")))
		       (forward-char 1)
		       (cond ((looking-at "T") (princ "type: "))
			     ((looking-at "C") (princ "type class: "))
			     ((looking-at "B") (princ "Built-in type: "))
			     ((looking-at "U") (princ "User defined type: "))
			     ((looking-at "P") (princ "procedure: "))
			     ((looking-at "I") (princ "iterator: "))
			     )
		       (forward-char 1)
		       (setq from-point (point))
		       (if till-dollar (re-search-forward "\\$")
			   (re-search-forward "\n"))
		       (forward-char -1)
		       (princ (buffer-substring from-point (point)))
		       (terpri) 
		       (cond (till-dollar (re-search-forward "\n")  (terpri)))
		       (setq from-point (point))
		       (re-search-forward clu-doc-start (point-max) 'to-end)
		       (forward-char -1)
		       (princ (buffer-substring from-point (point)))
		       (print-help-return-message))
		   )))
	))
;;;
;;;  ADD USER DEFINED FILES
;;;
(defvar clu-doc-temp-buffer "*clu-temp*" "Name of clu-doc temp buffer")
(defun goto-next-abstraction ()
    "Goto next occurance of  <identifier> = proc/iter/cluster . If not found
then return nil, else return the type of abst. (string) and position cursor at
begining of line of identifier"
    (let ((abst-regexp (concat bol identifier sep "=" sep abstraction))
	  (result nil)
	  from-point to-point)
	(cond ((re-search-forward abst-regexp nil t)
	       (setq to-point (point))
	       (re-search-backward "[ \t=\n]")
	       (setq result (buffer-substring (1+ (point)) to-point))
	       (goto-char to-point)
	       (re-search-backward abst-regexp)))
	result))

(defun get-next-identifier ()
    "Starting from current point, move forward and return next identifier.
If current point is in middle of identifier ID, return rest of ID"
    (let (from-point to-point)
	(re-search-forward "[a-z_A-Z]")
	(setq from-point (1- (point)))
	(re-search-forward "[^0-9a-zA-Z_]")
	(buffer-substring from-point (1- (point)))))

(defun handle-proc (start-point internal)
    "Handle procedure defined after START-POINT (== current point). INTERNAL,
if non-nil, is the name of the cluster"
    (let ((name (get-next-identifier))
	  eop ) ;; end of procedure 
	(princ clu-doc-start)
	(if (not internal)
		(princ (concat "UP" (upcase name) "\n"))
	    (princ (concat "OU" (upcase internal) "$" (upcase name) "\n")))
	(if (null (re-search-forward (concat bol "end[ \t\n]+" name) nil t))
		(error "Procedure %s does not terminate with \"end %s\"" 
		       name name))
	(setq eop (point))
	;; search of end of header == where the code begins
	(goto-char start-point)
	(re-search-forward "\\bproc\\b")
	(forward-sexp 1)
	(if (re-search-forward "\\breturns\\b" eop t) (forward-sexp 1))
	(if (re-search-forward "\\bsignals\\b" eop t) (forward-sexp 1))
	(re-search-forward (concat bol identifier) eop)
	(beginning-of-line)
	(princ (buffer-substring start-point (point)))))

(defun handle-iter (start-point internal)
    "Handle iterator defined after START-POINT (== current point). INTERNAL,
if non-nil, is the name of the cluster"
    (let ((name (get-next-identifier))
	  eoi ) ;; end of iterator
	(princ clu-doc-start)
	(if (not internal)
		(princ (concat "UI" (upcase name) "\n"))
	    (princ (concat "OU" (upcase internal) "$" (upcase name) "\n")))
	(if (null (re-search-forward (concat bol "end[ \t\n]+" name) nil t))
		(error "Iterator %s does not terminate with \"end %s\"" 
		       name name))
	(setq eoi (point))
	;; search of end of header == where the code begins
	(goto-char start-point)
	(re-search-forward "\\biter\\b")
	(forward-sexp 1)
	(if (re-search-forward "\\byields\\b" eoi t) (forward-sexp 1))
	(if (re-search-forward "\\bsignals\\b" eoi t) (forward-sexp 1))
	(re-search-forward (concat bol identifier) eoi)
	(beginning-of-line)
	(princ (buffer-substring start-point (point)))))

(defun get-ops (ops)
    "Return list of cluster operations. Set variable end-point dynamically"
    (while (looking-at "[ \t]*\\(%\\|\n\\)")
	   (re-search-forward "\n")) ;; skip comments and empty lines
    (cond ((looking-at (concat sep identifier sep "="))  ;; end of operat.
	   (setq end-point (point))  ;; dynamic binding -- yakkk
	   ops)
	  ((looking-at (concat sep "where\\b"))
	   (re-search-forward (concat bol identifier sep "="))
	   (beginning-of-line)
	   (setq end-point (point))
	   ops)
	  ((looking-at (concat sep ","))   ;;  skip ","
	   (re-search-forward ",")
	   (get-ops ops))
	  (t  (get-ops (cons (get-next-identifier) ops)))))

(defun handle-cluster (start-point)
    (let ((cluster-name (get-next-identifier))
	  (ops nil) ;; operations of this cluster
	  end-point ;; to be set dynamically to end of cluster header
	  eoc ) ;; end of cluster 
	(princ (concat clu-doc-start "UT" (upcase cluster-name) "\n"))
	(if (null (re-search-forward (concat bol "end[ \t\n]+" cluster-name)
				     nil t))
		(error "Cluster %s does not terminate with \"end %s\"" 
		       cluster-name cluster-name))
	(setq eoc (point))
	;; search of end of header == where the code begins
	(goto-char start-point)
	(re-search-forward "\\bcluster\\b")
	(if (looking-at "[ \t\n]*\\[") (forward-sexp 1)) ;; if parameterized
	(re-search-forward "\\bis\\b")
	;; collect names of operations (assumes at least one exists)
	(setq ops (nreverse (get-ops ops)))    ;; also sets end-point
	(princ (buffer-substring start-point end-point)) 
	;; extract documentation for ops
	(while (not (null ops))
	    (goto-char start-point)
	    (if (null (re-search-forward (concat bol (car ops) sep "=") eoc t))
		    (error 
		     "Operation %s declared but not found inside cluster %s"
		     (car ops) cluster-name))
	    (cond ((looking-at (concat sep "proc"))
		   (re-search-backward (concat bol (car ops)))
		   (handle-proc (point) cluster-name))
		  ((looking-at (concat sep "iter"))
		   (re-search-backward (concat bol (car ops)))
		   (handle-iter (point) cluster-name))
		  (t (error 
		      "Expecting iter or proc %s but not found in cluster %s"
		      (car ops) cluster-name)))
	    (setq ops (cdr ops))
	    )
	(goto-char eoc) ;; avoid treating cluster body as indep. procs/iters
	))

(defun clu-doc-add-directory (dname)
    "Add specifications of all the Clu files in directory DNAME to the current
session of the CLU manual on-line help facility. The Clu files are all the 
files whose suffix matches clu-doc-file-suffix"
    (interactive "DGive name of directory with CLU files: ")
    (let ((files (directory-files dname t (concat clu-doc-file-suffix "$"))))
	(mapcar 'clu-doc-add-file files)
	(message "Done adding CLU files from directory %s\t" dname))
    )

(defun clu-doc-add-file (fname)
    "Takes a syntactically correct Clu file FNAME, extracts the specifications
and adds them to the current session of the Clu help facility."
    (interactive "fGive name of CLU file: ")
    (clu-doc-parse-file fname)
    (clu-doc-load-buffer clu-doc-temp-buffer)
    (message "File %s added to current CLU manual." fname)
    )

(defun clu-doc-parse-file (fname)
    "Take name of CLU file (FNAME), and extract its specifications (in CLU-DOC
format) into clu-doc-temp-buffer (erasing previous contents in this buffer)."
    (let ((file-is-loaded (get-buffer fname))
	  (buf (find-file-noselect fname))
	  (temp-buffer-show-hook (function (lambda (b)))) ;; do not display
	  type
	  from-point code-point
	  (bol "^[ \t]*")  ;; regexp for beggining of line
	  (sep "[ \t\n]*") ;; regexp for separators
	  (identifier "[a-z_A-Z][a-zA-Z_0-9]*")
	  (abstraction "\\(proc\\|iter\\|cluster\\)\\b"))
	(save-excursion
	    (set-buffer buf)
	    (goto-char (point-min))
	    (setq case-fold-search t)
	    (with-output-to-temp-buffer clu-doc-temp-buffer
		(while (setq type (goto-next-abstraction))
		    (cond ((equal type "proc") (handle-proc (point) nil))
			  ((equal type "iter") (handle-iter (point) nil))
			  ((equal type "cluster") (handle-cluster (point)))
			  )))
	    )
	(if (null file-is-loaded) (kill-buffer buf))
	))

(defun clu-doc-load-buffer (doc-buf)
    "DOC-BUF should contain file in CLU-DOC format. Adds DOC-BUF contents 
into the current CLU manual"
    (require-doc-file-in-buffer)
    (set-buffer clu-doc-buffer)
    (setq buffer-auto-save-file-name nil) ;; don't save
    (goto-char (point-max))
    (setq from-point (point))
    (insert-buffer doc-buf)
    (set-buffer-modified-p nil) ;; don't save this buffer
    (goto-char from-point)
    (while (search-forward clu-doc-start nil t)
	(setq code-point (point))
	(forward-char clu-doc-key-length)
	(setq from-point (point))
	(forward-sexp 1)
	(setq clu-symbols-doc
	      (cons
	       (cons (buffer-substring from-point (point)) 
		     code-point)
	       clu-symbols-doc))
	)
    )

(defun clu-doc-load-file (dumped-file)
    "Load DUMPED-FILE into the current session of the CLU manual. 
DUMPED-FILE should have been created by clu-doc-dump-file"
    (interactive "fGive name of a CLU dump file: ")
    (setq dumped-file (expand-file-name dumped-file))
    (let ((file-is-loaded (get-buffer dumped-file))
	  (buf (find-file-noselect dumped-file)))
	(clu-doc-load-buffer buf)
	(if (null file-is-loaded) (kill-buffer buf))
	(message "Done loading CLU dump file %s" dumped-file)
	))

(defun clu-doc-dump-file (clu-file dump-file)
    "Extracts specifications from CLU-FILE and appends them to DUMP-FILE (in
CLU-DOC format). DUMP-FILE can be later loaded using  M-x clu-doc-load-file.
Asks user whether these specifications should also be added to the current 
session of the Clu manual."
    (interactive "fGive name of a CLU file: \nFFile to append this dump to: ")
    (clu-doc-parse-file clu-file)
    (save-excursion
	(set-buffer clu-doc-temp-buffer)
	(write-region (point-min) (point-max) dump-file t 'no-show))
    (if (y-or-n-p "Also add to current CLU manual? ")
	    (clu-doc-load-buffer clu-doc-temp-buffer))
    (message "Dumped specifications of CLU file %s into %s" clu-file dump-file)
    )

(defun clu-doc-total-dump (dfile)
    "Dump the whole current Clu help manual into a new DFILE, also generate 
-keys.el file. The new DFILE can be used later in lieu of CLU-DOC file"
    (interactive "FGive name for a new CLU DOC file: ")
    (setq dfile (expand-file-name dfile))
    (if (file-exists-p dfile) (error "File %s exists!  Aborting." dfile))
    (require-doc-file-in-buffer)
    (save-excursion
	(set-buffer clu-doc-buffer)
	(write-file dfile)
	(clu-doc-dump-keys dfile clu-symbols-doc clu-general-doc "dump")))
