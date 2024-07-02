
\[ ret \] -->  `ret <expr>;`
\[ disp \] --> `disp("<string>");`

\[ prog \] --> \[ stmt \] \*
\[ expr \] --> {
			`int_lit`
			 `string`
			 `ident`
		   } 
\[ stmt \] --> {
			`ret <expr>;`
			`type: ident = <expr>;`
		   }