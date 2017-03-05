#lang racket

(struct animal (name attributes)) 

(define dog (animal "Dog" (list "mammal" "domestic" "long tail" "bau bau")))
(define cat (animal "Cat" (list  "meow and purr" "domestic" "long tail" "mammal")))
(define hamster (animal "Hamster" (list "mammal" "domestic" "small,in cage")))
(define rabbit (animal "Rabbit" (list "mammal" "domestic" "carrots")))
(define tiger (animal "Tiger" (list "mammal" "wild" "cat-family member")))
(define wolf (animal "Wolf" (list "mammal" "wild" "in wood mainly")))
(define owl (animal "Owl" (list "bird" "wild" "night creature")))
(define eagle (animal "Eagle" (list "bird" "wild" "predator")))
(define parrot (animal "Parrot" (list "bird" "domestic" "many colours")))
(define salamander (animal "Salamander" (list "amphibians" "wild" "long tail" "slender body")))
(define frog (animal "Frog" (list "amphibians" "wild" "jump high" "amazing sense of hearing")))

(define initial-animals (list dog cat hamster rabbit tiger wolf owl eagle parrot salamander frog))

(define (filter-by-attributes attribute answer animals) ;
    (filter (λ (animal)
              (if answer
                  (member attribute (animal-attributes animal))
                  (not (member attribute (animal-attributes animal)))
              )
            )
     animals
     )
 )

(define (get-all-attributes available-animals) ;
  (set->list
   (list->set
    (foldr (λ (animal temp)
          (append (animal-attributes animal) temp))
            null available-animals
     )
    )
   )
) 

(define (add-new-animal all-animals current) ;
  (printf "Can you please tell me what is your animal?\n")
  (let ([name (read)])
    (printf "What is an identifying charachetistic?\n")
     (let* ([attribute (format "~s" (read))]
            [new-animals (cons (animal name (cons attribute current)) all-animals)])
       (guess-the-animal new-animals
                         new-animals
                         (get-all-attributes new-animals)
                         null))))

(define (guess-the-animal all-animals current-animals current-attributes current) ;
  (cond [(null? current-animals)
         (printf "I have no idea!You tricked me this time!\n")
         (add-new-animal all-animals current)]
        [(null? (rest current-animals))
                 (printf "I know!Your animal is the ~s\n y or n\n"
                         (animal-name (first current-animals)))
                 (let ([answer (read)])
                   (if (not (equal? answer `y))
                       (add-new-animal all-animals current)
                       (begin (printf "I win! Do you want to play again?\n")
                       (let ([answer (read)])
                         (if (equal? answer 'y)
                            (guess-the-animal all-animals all-animals (get-all-attributes all-animals) null)
                            (printf "Thank you for playing and hope to see you next time!"))))))]

         [else (printf "Does your animal have/is a/make the sound/can/live/eat/ ~s" (first current-attributes))
               (let ([answer (read)])
                 (guess-the-animal all-animals
                                   (filter-by-attributes (first current-attributes)
                                                         (equal? answer `y)
                                                         current-animals)
                                   (rest current-attributes)
                                   (if (equal? answer `y)
                                       (cons (first current-attributes) current)
                                       current
                                       )
                                   )
                 )
               ]
         )
  )

(printf "Welcome to Asparuh's Guess the Animal Game!You can choose from cat,dog,hamster,rabbit,tiger,wolf,owl,eagle,parrot,salamander,frog,
 or maybe you would surprise me with a new animal.. Well,let's find out!\n")
(guess-the-animal initial-animals initial-animals (get-all-attributes initial-animals) null)



                              
                                