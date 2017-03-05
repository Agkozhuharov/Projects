
import System.IO
-- definition of the animals that are going to be used in displayAnimals
type Animal = (String,String)
cat :: Animal
cat = ("Cat","Mammal")
dog :: Animal
dog = ("Dog","Mammal")
hamster :: Animal
hamster = ("Hamster","Mammal")
rabbit :: Animal
rabbit = ("Rabbit","Mammal")
tiger :: Animal
tiger = ("Tiger","Mammal")
wolf :: Animal
wolf = ("Wolf","Mammal")
owl :: Animal
owl = ("Owl","Bird")
eagle :: Animal
eagle = ("Eagle","Bird")
parrot :: Animal
parrot = ("Parrot","Bird")
salamander :: Animal
salamander = ("Salamander","Amphibian")
frog :: Animal
frog = ("Frog","Amphibian")
type AllAnimals = [Animal]
allanimals :: AllAnimals
allanimals = [cat,dog,hamster,rabbit,tiger,wolf,owl,eagle,parrot,salamander,frog]

-- function that displays all the animals that can be chosen by the user    
displayAnimals :: [(String,String)] -> IO()
displayAnimals [] = putStrLn("")
displayAnimals ((x,y):xs) = do
    putStrLn x
    displayAnimals xs

-- used to help display the possible "suspects" left after determining the user's animal characteristic
filterAnimals :: [(String,String)] -> String -> [(String,String)]
filterAnimals [] characteristic = []
filterAnimals ((x,y):xs) characteristic
 | y == characteristic = [(x,y)] ++ filterAnimals xs characteristic
 | otherwise = filterAnimals xs characteristic
 
 
-- Main question initializing function
findYourAnimal :: IO()
findYourAnimal = do
    putStrLn("Starting a new game ........... \n")
    putStr("Is your animal a mammal? ")
    line <- getLine
    if (line == "y") 
    then solveForMammals 
    else solveForOthers 
    
-- function that repeats the process or lets the user exit after his animal has been found
repeatORbye :: IO()
repeatORbye = do
    putStr("Would you like to continue? ") 
    line <- getLine
    if (line == "y") 
    then do
     putStrLn("Very well then.")
     findYourAnimal
    else putStrLn("OK then, see you next time!")
    
-- function that helps solve if user is thinking of a mammal   
solveForMammals :: IO()
solveForMammals = do
    putStrLn("Possible animals left:")
    displayAnimals (filterAnimals allanimals "Mammal")
    putStr("Is your animal domestic? ")
    line <- getLine
    if (line == "y") 
    then solveForDomestic 
    else solveForWild
    
 -- function that solves if user is thinking of a domestic mammal
solveForDomestic :: IO()
solveForDomestic = do
    putStr("Does your animal have a long tail? ") 
    line <- getLine
    if (line == "y") 
    then catOrdog
    else hamsterOrrabbit
   
-- solves for cat or dog
catOrdog :: IO()
catOrdog = do
    putStr("Does your animal purr and meow? ") 
    line <- getLine
    if (line == "y") 
    then do 
     putStrLn("Then your animal must be a CAT.") 
     repeatORbye
    else do 
     putStrLn("Then your animal must be a DOG.") 
     repeatORbye

-- solves for hamster or rabbit
hamsterOrrabbit :: IO()
hamsterOrrabbit = do
    putStr("Does your animal have big ears and love for carrots? ") 
    line <- getLine
    if (line == "y") 
    then do 
     putStrLn("Then your animal must be a RABBIT.") 
     repeatORbye
    else do 
     putStrLn("Then your animal must be a HAMSTER.") 
     repeatORbye
     
-- solves for wild mammals
solveForWild :: IO()
solveForWild = do
    putStr("Is your animal a member of the cat family? ") 
    line <- getLine
    if (line == "y") 
    then do
     putStrLn("Then your animal must be a TIGER!") 
     repeatORbye
    else do 
     putStrLn("Then your animal must be a WOLF") 
     repeatORbye
    
-- helps solve for non mammal animals
solveForOthers :: IO()
solveForOthers = do
    putStr("Can your animal fly? ")
    line <- getLine
    if (line == "y") 
    then solveForBirds
    else solveForAmphibians
    
-- helps solve for birds
solveForBirds :: IO()
solveForBirds= do
    putStrLn("Possible animals left:")
    displayAnimals (filterAnimals allanimals "Bird")
    putStr("Is your animal nocturnal (a creature of the night)? ") 
    line <- getLine
    if (line == "y") 
    then do 
     putStrLn("Then your animal must be an OWL.") 
     repeatORbye
    else parrotOReagle
-- solves for parrot or eagle
parrotOReagle :: IO()
parrotOReagle = do
    putStr("Is your animal a predator? ") 
    line <- getLine
    if (line == "y") 
    then do 
     putStrLn("Then your animal must be an EAGLE.") 
     repeatORbye
    else do 
     putStrLn("Then your animal must be a PARROT.") 
     repeatORbye
    
-- solves for amphibians
solveForAmphibians :: IO()
solveForAmphibians = do
    putStrLn("Possible animals left:")
    displayAnimals (filterAnimals allanimals "Amphibian")
    putStr("Can your animal jump high? ") 
    line <- getLine
    if (line == "y") 
    then do 
     putStrLn("Then your animal must be a FROG.") 
     repeatORbye
    else do 
     putStrLn("Then your animal must be a SALAMANDER.") 
     repeatORbye
     
-- Starts the game     
initialize :: IO()
initialize = do
    putStrLn("Welcome to the Asparuh and Ivan HASKELL GUESS THE ANIMAL GAME! ")
    putStrLn("Here is the list of animals you can choose from: ")
    displayAnimals(allanimals)
    findYourAnimal

main :: IO()
main = do
    hSetBuffering stdout NoBuffering
    initialize




