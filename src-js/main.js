class MenuEventHandler {
  name;
  startButton;
  newGameButton;
  quitButton;
  constructor() {
    this.name = "Main Menu";
    this.startButton = document.querySelector(".start-button");
    this.newGameButton = document.querySelector(".newgame-button");
    this.quitButton = document.querySelector(".exit-button");

    this.startButton.addEventListener("click", this.startGame);
    this.newGameButton.addEventListener("click", this.newGame);
    this.quitButton.addEventListener("click", this.quitGame);
  }

  startGame() {
    console.log("Here call Rust function to initialize game.");
  }
  newGame() {
    console.log("Here call Rust function to initialize new game.");
  }
  quitGame() {
    console.log("Here call Rust function to quit game.");
  }
}
const menu = new MenuEventHandler();
