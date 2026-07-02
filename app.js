
const code1 = "a3çle" // codigo para aceder à imagem paint.net
const code2 = "lco7w" // codigo para aceder ao jogo
const code3 = "4pwod" // codigo para riddle com midnight gospel
const code_final = "3sg4y" // codigo para reprogramar a bussola
const code_phi = "abc23"

const extra_code = "ak3m8"
const schematic_code = "hj4bi" //obtido depois de ver o grafiti


function createPuzzleElements() {
    const debugElement = document.createElement("p");
    debugElement.id = "debug";
    document.body.insertBefore(debugElement, document.body.firstChild);

    const popupElements = [
        { id: "1", className: "big-box hidden", text: "Top sabes escrever códigos já percebi" },
        { id: "2", className: "big-box hidden", text: "Mas e saber programar?" },
        { id: "3", className: "big-box hidden", text: "Para a próxima parte tens 2 possibilidades" },
        { id: "4", className: "big-box hidden", text: "Aprender a controlar event listeners" },
        { id: "5", className: "big-box hidden", text: "Ou brute force (aconselho a teres calma)" },
        { id: "6", className: "big-box hidden", text: "The time has passed, and it is yet to come" },
        { id: "7", className: "big-box hidden", text: "Espertinho, mas esse código foi só uma sequencia aleatória para te fazer perder tempo" }
    ];

    popupElements.forEach(({ id, className, text }) => {
        const element = document.createElement("div");
        element.id = id;
        element.className = className;
        element.textContent = text;
        document.body.appendChild(element);
    });

    const finalCodeLink = document.createElement("a");
    finalCodeLink.href = "final_code.ino";
    finalCodeLink.className = "small-box hidden";
    finalCodeLink.id = "999";
    finalCodeLink.download = "Código final";
    finalCodeLink.textContent = ":)";
    document.body.appendChild(finalCodeLink);

    const clueLink = document.createElement("a");
    clueLink.href = "clue.png";
    clueLink.className = "small-box hidden";
    clueLink.id = "clueImage";
    clueLink.download = "stuff";
    clueLink.textContent = "?";
    document.body.appendChild(clueLink);

    const phiLink = document.createElement("a");
    phiLink.href = "foto.jpeg";
    phiLink.className = "small-box hidden";
    phiLink.id = "phiImage";
    phiLink.download = "find_where";
    phiLink.textContent = "?";
    document.body.appendChild(phiLink);

    const riddleLink = document.createElement("a");
    riddleLink.href = "riddle.txt";
    riddleLink.className = "small-box hidden";
    riddleLink.id = "download";
    riddleLink.textContent = "!";
    document.body.appendChild(riddleLink);

    const gameLink = document.createElement("a");
    gameLink.href = "game.zip";
    gameLink.className = "big-box hidden";
    gameLink.id = "game";
    gameLink.download = "Back_Again.zip";
    gameLink.textContent = "Guess Who's back";
    document.body.appendChild(gameLink);
}

createPuzzleElements();

debug = document.getElementById("debug")
function print(a){
    debug.textContent = a;
}


// Add stars layout to bckg
const TILE_COUNT = 50;

for (let i = 0; i < TILE_COUNT; i++) {
  const tile = document.createElement("div");
  tile.className = "tile";

  const size = 30 + Math.random() * 120;
  const x = Math.random() * window.innerWidth;
  const y = Math.random() * window.innerHeight;

  tile.style.width = `${size}px`;
  tile.style.height = `${size}px`;
  tile.style.left = `${x}px`;
  tile.style.top = `${y}px`;

  tile.style.backgroundImage = "url('estrela.png')";
  tile.style.backgroundPosition = `50% 50%`;

  document.body.appendChild(tile);
}


const input = document.getElementById("code");
const btn = document.getElementById("close-popup")
btn.addEventListener('click', () => {
        box.classList.add("hidden")
        resolver?.();
        resolver = null;
    });

input.addEventListener("keydown", (e) => {
  if (e.key==='Enter'){
    const codigo = input.value;

    if (codigo === code1) clue_image()
    if (codigo === code2) game_download()
    if (codigo === code3) riddle()
    if (codigo === code_final) final_code()
    if (codigo === extra_code) waste_time()
    if (codigo === schematic_code) schematics()
    if (codigo === code_phi) phi()

    // if (![code1,code2,code3,code_final].includes(codigo)) treasure_map(codigo);
  }
});


let resolver = null;
function show_box(num){
    box = document.getElementById(num)
    box.classList.remove("hidden")
    
    if (num == 5){
        btn.style.left = Math.random() * 100 + '%';     
        btn.style.top = Math.random() * 100 + '%'; 
        btn.addEventListener("mouseover", (e)=>{
            setTimeout(function (){
                btn.style.left = 0.01 + Math.random() * 90 + '%';     
                btn.style.top = 0.01 + Math.random() * 90 + '%';     
        }, 50);   
        })
    }
    if (num == 6){
        box.style.zIndex = 12
    }

    btn.classList.remove('hidden')
    
    return new Promise(resolve => {
        resolver = resolve;
    });
}




function clue_image(){
    input.remove()
    document.querySelector('#clueImage').classList.remove('hidden')
}


async function waste_time(){
    await show_box(7)
}


async function game_download(){
    input.remove()
    await show_box(1)
    await show_box(2)
    await show_box(3)
    await show_box(4)
    await show_box(5)
    
    btn.classList.add('hidden')



    document.querySelector('#game').classList.remove('hidden')
}



async function riddle(){
    input.remove()

    const now = new Date()
    if ((now.getHours() == 0 && now.getMinutes() == 0) || (now.getHours() == 0 && now.getMinutes() == 1) || (now.getHours() == 23 && now.getMinutes() == 59)){
        document.querySelector('#download').classList.remove('hidden')
    }
    else {
        console.log(now.getMinutes())
        console.log(now.getHours())
        const map = document.createElement("div")
        map.className = "big-img"
        map.style.backgroundImage = "url('gospel.png')"
        document.body.appendChild(map)

        await show_box(6)
    }

}




async function final_code(){
    input.remove()
    document.querySelectorAll(".tile").forEach(t=>t.remove())

    await show_box(999)
}


async function schematics() {
    const sch = document.createElement("div")
    sch.className = "small-img"
    sch.style.backgroundImage = "url('esquematico.jpeg')"
    document.body.appendChild(sch)
}


async function phi(){
    input.remove()
    document.querySelector('#phiImage').classList.remove('hidden')
}