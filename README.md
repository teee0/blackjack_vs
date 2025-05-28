bibliografie:
https://stackoverflow.com/questions/29859796/c-auto-vs-auto
https://stackoverflow.com/questions/60386792/c20-comparison-warning-about-ambiguous-reversed-operator
https://stackoverflow.com/questions/38942013/declaring-variables-in-header-files-c
https://www.wikihow.com/Play-Blackjack
https://bicyclecards.com/how-to-play/blackjack/
https://stackoverflow.com/questions/41753358/creating-custom-exceptions-in-c
https://www.geeksforgeeks.org/advanced-c-virtual-constructor/
https://www.geeksforgeeks.org/dynamic-_cast-in-cpp/
https://stackoverflow.com/questions/3901630/in-a-i-vector-size-loop-condition-is-size-called-each-iteration
https://www.geeksforgeeks.org/advanced-c-virtual-copy-constructor/
https://stackoverflow.com/questions/3279543/what-is-the-copy-and-swap-idiom
https://refactoring.guru/design-patterns/factory-comparison
https://www.geeksforgeeks.org/factory-method-pattern-c-design-patterns/
https://stackoverflow.com/questions/9323903/most-efficient-elegant-way-to-clip-a-number

*ierarhie de clase:
baza: Player
derivata1: RealPlayer  -- jucător real ce joacă de la tastatură
derivata2: GhostPlayer -- jucător fantomă asociat unui RealPlayer ce gestionează alt set de cărți al jucătorului
derivata3: Dealer      -- bot


*alte clase:
Card:
Hand:
Deck:

*funcție virtuală pură: choice -- pt player e prompt la tastatură
												  		 -- pt dealer este un algoritm
												show_cards --simpla afisare ce tine cont ca a doua carte a dealerului este ascunsa in prima runda

*funcțiile de afișare sunt constante

*clasa cu pointeri spre alte clase ca atribut este Runda



