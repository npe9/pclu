;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Mode for editing clu programs
;;;
;;; Sanjay Ghemawat
;;; sanjay@lcs.mit.edu
;;; August 9, 1992
;;;
;;; (sanjay@lcs.mit.edu, September 23, 1992)
;;;	- Define indent-line-function so that indent-region works correctly.
;;;	- If in middle of line when indenting, stay there.

;;; Indentation variables
(defvar clu-indent-level 4
  "*Default indentation for clu.")

(defvar clu-resignal-indent 4
  "*Extra indentation for resignal line.")

(defvar clu-except-indent 1
  "*Extra indentation for lines starting with EXCEPT.")

(defvar clu-when-tag-indent -2
  "*Extra indentation for WHEN, OTHERS, and TAG lines.")

(defvar clu-begin-indent 2
  "*Extra indentation for lines starting with BEGIN.")

(defvar clu-end-indent -2
  "*Extra indentation for lines starting with END.")

(defvar clu-then-indent -2
  "*Extra indentation for THEN lines.")

(defvar clu-else-indent -2
  "*Extra indentation for ELSE lines.")

(defvar clu-elseif-indent -2
  "*Extra indentation for ELSEIF lines.")

;;; Customization hook

(defvar clu-mode-hook nil
  "Hooks for Clu mode.")

;;; Syntax table for Clu programs

(defvar clu-mode-syntax-table nil
    "Syntax table for Clu-mode buffers.")
(if clu-mode-syntax-table
    ()
  (let ((tbl (make-syntax-table)))
    (modify-syntax-entry ?\'	"\"   " tbl)
    (modify-syntax-entry ?\"	"\"   " tbl)
    (modify-syntax-entry ?\_	"w   " tbl)
    (modify-syntax-entry ?\(	"()  " tbl)
    (modify-syntax-entry ?\)	")(  " tbl)
    (modify-syntax-entry ?\{	"(}  " tbl)
    (modify-syntax-entry ?\}	"){  " tbl)
    (modify-syntax-entry ?\[	"(]  " tbl)
    (modify-syntax-entry ?\]	")[  " tbl)
    (modify-syntax-entry ?\%	"<   " tbl)
    (modify-syntax-entry ?\n	">   " tbl)
    (modify-syntax-entry ?\:	".   " tbl)
    (modify-syntax-entry ?\,	".   " tbl)
    (modify-syntax-entry ?\.	".   " tbl)
    (modify-syntax-entry ?\$	".   " tbl)
    (modify-syntax-entry ?\=	".   " tbl)
    (modify-syntax-entry ?\@	".   " tbl)
    (modify-syntax-entry ?\~	".   " tbl)
    (modify-syntax-entry ?\*	".   " tbl)
    (modify-syntax-entry ?\/	".   " tbl)
    (modify-syntax-entry ?\|	".   " tbl)
    (modify-syntax-entry ?\+	".   " tbl)
    (modify-syntax-entry ?\-	".   " tbl)
    (modify-syntax-entry ?\<	".   " tbl)
    (modify-syntax-entry ?\>	".   " tbl)
    (modify-syntax-entry ?\&	".   " tbl)
    (setq clu-mode-syntax-table tbl)
    ))

;;; Keymap for editing clu programs

(defvar clu-mode-map nil
    "Keymap used in Clu mode.")
(if clu-mode-map ()
  (let ((map (make-sparse-keymap)))
    (define-key map "\^i"		'clu-indent-line)
    (define-key map "\r"		'clu-newline)
    (setq clu-mode-map map)))

;;; Start clu mode

(defun clu-mode ()
  "This mode supports program development in Clu.
\\{clu-mode-map}

You can get help on CLU by using the emacs help key (normaly
Control-h) followed by one of the keys:

1. 'd' -- for help on Clu symbols (types or operations. Examples: int$mul, 
	  array, ARRAY$FILL, FILE_NAME, stream$primary_output, string$s2ac)
2. 'g' -- for general Clu documentation topics (Examples: TERMINAL_I/O, FILE,
	  own_variables, CLUSTERS, syntactic_sugar).
3. '/' -- for Clu apropos (give partial name, like FILE, and get a list of all
	  the Clu symbols and topics that include that name).

EXAMPLE:
  You type  		 C-h d
  Emacs prompts with	 Describe CLU symbol: 
  You type		 oneof$make_  <RETURN>
  Emacs splits your screen and gives help about the operation ONEOF$MAKE_

Note that:
- case is unimportant. 
- The help text is taken from the Clu manual (Appendix A in the 6.170 book).
- Symbol completion works (hit the TAB key); for example, if you type  ARRAY$
  followed by TAB, you'll get a list of possible completions (all the 
  operations of ARRAY in this case).
- If the cursor is placed on a Clu symbol when you call C-h d, emacs would 
  propose that symbol as default; for example, if your cursor is placed 
  someplace on \"ARRAY[foo]$FILL\" in your file, and you type C-h d , emacs 
  would prompt:   \"Describe CLU symbol (default ARRAY$FILL): \", typing RETURN
  would select this default."
  (interactive)
  (kill-all-local-variables)

  (use-local-map clu-mode-map)
  (set-syntax-table clu-mode-syntax-table)
  (setq major-mode 'clu-mode)
  (setq mode-name "Clu")

  (make-local-variable 'comment-column)
  (setq comment-column 41)
  (make-local-variable 'end-comment-column)
  (setq end-comment-column 75)
  (make-local-variable 'paragraph-start)
  (setq paragraph-start (concat "^$\\|" page-delimiter))
  (make-local-variable 'paragraph-separate)
  (setq paragraph-separate paragraph-start)
  (make-local-variable 'indent-line-function)
  (setq indent-line-function 'clu-indent-line)
  (make-local-variable 'require-final-newline)
  (setq require-final-newline t)
  (make-local-variable 'comment-start)
  (setq comment-start "% ")
  (make-local-variable 'comment-end)
  (setq comment-end "\n")
  (make-local-variable 'comment-column)
  (setq comment-column 41)
  (make-local-variable 'comment-start-skip)
  (setq comment-start-skip "%* *")

  ;; Necessary since comments are terminated by newlines
  (make-local-variable 'parse-sexp-ignore-comments)
  (setq parse-sexp-ignore-comments nil)

  (run-hooks 'clu-mode-hook)
  )

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Block delimiters
;;;

;; List of defun starters
(defconst clu-defun-starter-list
  (list "proc"
	"iter"
	"cluster"
	"equates"
	))

;; Combined list of all block starters
(defconst clu-block-starter-list
  (append (list "while"
		"for"
		"if"
		"begin"
		"enter"
		"tagcase"
		"except"
		)
	  clu-defun-starter-list
	  ))

;; List of block enders
(defconst clu-block-ender-list
  (list "end"
	))

;; Combined list of all block delimiters
(defconst clu-block-delimiter-list
  (append clu-block-starter-list
	  clu-block-ender-list
	  ))

;; A list of keywords that cannot occur inside a list
;; Does not have to be complete
(defconst clu-not-in-list-list
  (list	"begin"
	"break"
	"continue"
	"else"
	"elseif"
	"end"
	"except"
	"exit"
	"for"
	"if"
	"others"
	"own"
	"resignal"
	"return"
	"signal"
	"tag"
	"tagcase"
	"then"
	"when"
	"while"
	"yield"
	))

(defconst clu-defun-regexp
  (concat "^[ \t]*[a-zA-Z0-9_]+[ \t]*=[ \t]*\\("
	  (mapconcat 'identity clu-defun-starter-list "\\|")
	  "\\)\\b"))

(defconst clu-block-starter-regexp
  (concat "\\b\\("
	  (mapconcat 'identity clu-block-starter-list "\\|")
	  "\\)\\b"))

(defconst clu-block-ender-regexp
  (concat "\\b\\("
	  (mapconcat 'identity clu-block-ender-list "\\|")
	  "\\)\\b"))

(defconst clu-block-delimiter-regexp
  (concat "\\b\\("
	  (mapconcat 'identity clu-block-delimiter-list "\\|")
	  "\\)\\b"))

(defconst clu-not-in-list-regexp
  (concat "^[ \t]*\\b\\("
	  (mapconcat 'identity clu-not-in-list-list "\\|")
	  "\\)\\b"))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;
;;; Compute extra indentation based on current line
;;; REQUIRES at beginning of line

(defun clu-special-offset ()
  (if (looking-at "[ \t]*\\([a-zA-Z]+\\)\\>")
      (let ((word (buffer-substring (match-beginning 1) (match-end 1))))
	(cond
	 ((equal word "begin")
	  (symbol-value 'clu-begin-indent))
	 ((equal word "end")
	  (symbol-value 'clu-end-indent))
	 ((equal word "then")
	  (symbol-value 'clu-then-indent))
	 ((equal word "else")
	  (symbol-value 'clu-else-indent))
	 ((equal word "elseif")
	  (symbol-value 'clu-elseif-indent))
	 ((equal word "except")
	  (symbol-value 'clu-except-indent))
	 ((or (equal word "when")
	      (equal word "tag")
	      (equal word "others"))
	  (symbol-value 'clu-when-tag-indent))
	 ((equal word "resignal")
	  (symbol-value 'clu-resignal-indent))
	 (t 0)
	 )
	)
    0))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;
;;; Compute indentation for current line
;;;

(defun clu-compute-indent ()
  (save-excursion
    (beginning-of-line)

    ;; Check to see if we are in a list of some form
    (let ((list-pos (clu-list-start)))
      (if list-pos
	  ;; Inside list. Use start of list for indentation
	  (save-excursion (goto-char (1+ list-pos)) (current-column))

	;; Find point for relative indentation.
	;; Adjust its indentation by nesting levels between relative point
	;; and current point. Keep track of clu-special-offset for each line

	(let (nesting special1 relative-indent special2)
	  ;; nesting		Indentation relative to indentation point
	  ;; relative-indent	Indentation of line containing indentation pt
	  ;; special1		Special offset for relative line
	  ;; special2		Special offset for this line
	  (save-excursion
	    (setq nesting (clu-find-indentation-point))
	    (setq special1 (clu-special-offset))
	    (setq relative-indent (progn (skip-chars-forward " \t")
					 (current-indentation)))
	    )

	  (setq special2 (clu-special-offset))
	  (+ (- relative-indent special1) nesting special2)
	  )
	))
    )
  )
      
;;;
;;; Indent the current line as Clu code.
;;;
(defun clu-indent-line ()
  (interactive)
  (save-excursion
    (beginning-of-line)
    (delete-horizontal-space)
    (indent-to (clu-compute-indent))
    )
  (if (bolp) (forward-to-indentation 0))
  )

;;;
;;; Re-indent current line and then make new line.
;;;
(defun clu-newline ()
  (interactive)
  (newline)
  (save-excursion
    (backward-char 1)
    (beginning-of-line)
    (delete-horizontal-space)
    (indent-to (clu-compute-indent))
    )
  (delete-horizontal-space)
  (indent-to (clu-compute-indent))
  )

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Indentation information

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;
;;; Find line relative to which indentation will be computed
;;; REQUIRES at beginning of line. Not inside list.
;;; RETURNS  indentation relative to found point
;;; EFFECTS  moves to indentation point
;;;

(defun clu-find-indentation-point ()
  (let ((nest 0)			;; Relative indentation found so far
	(done nil)			;; Done yet?
	)

    ;; Keep searching backwards sexp by sexp until we get to the
    ;; beginning of a line

    (while (not done)
      (backward-sexp 1)

      ;; Adjust nesting level if necessary
      (cond ((clu-in-comment-p)
	     (clu-start-of-comment))
	    ((looking-at clu-block-starter-regexp)
	     (setq nest (1+ nest)))
	    ((looking-at clu-block-ender-regexp)
	     (setq nest (1- nest)))
	    (t
	     nil)
	    )

      ;; Check if we are done yet	    
      (if (save-excursion (skip-chars-backward " \t") (bolp))
	  ;; At beginning of line. Use this as indentation point
	  (setq done t)
	)
      )

    (beginning-of-line)
    (* nest (symbol-value 'clu-indent-level))
    )
  )

;;; Is point inside comment?

(defun clu-in-comment-p ()
  (let ((state (parse-partial-sexp (save-excursion (beginning-of-line) (point))
				   (point))))
    (nth 4 state)))

;;; Is point inside string or character literal

(defun clu-in-string-p ()
  (let ((state (parse-partial-sexp (save-excursion (beginning-of-line) (point))
				   (point))))
    (nth 3 state)))

;;; Is point inside comment or string

(defun clu-in-comment-or-string-p ()
  (let ((state (parse-partial-sexp (save-excursion (beginning-of-line) (point))
				   (point))))
    (or (nth 3 state) (nth 4 state))))

;;;
;;; Return position of innermost list that contains (point).
;;; Return nil if not inside any list

(defun clu-list-start ()
  (save-excursion
    (let ((end (point)))
      (re-search-backward clu-not-in-list-regexp (point-min) 1)
      (nth 1 (parse-partial-sexp (point) end))
      )
    )
  )

;;;
;;; Search backwards for closest beginning of defun

(defun clu-beginning-of-defun ()
  (end-of-line)
  (re-search-backward clu-defun-regexp (point-min) 1)
  )

;;;
;;; Go to beginning of current comment

(defun clu-start-of-comment ()
  (beginning-of-line)
  (skip-chars-forward "^%'\"")
  (while (not (looking-at "%"))
    ;; At string start
    (forward-sexp 1)
    (skip-chars-forward "^%'\""))
  )

;;;
;;; Get the CLU documentation

(load-library "clu-doc")
