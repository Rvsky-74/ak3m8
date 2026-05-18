
const code1 = "a3çle" // codigo para aceder à imagem paint.net
const code2 = "lco7w" // codigo para aceder ao jogo
const code3 = "4pwod" // codigo para riddle com midnight gospel
const code_final = "3sg4y" // codigo para reprogramar a bussola

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
    if ((now.getHours() == 0 && now.getMinutes() == 0) || (now.getHours() == 22 && now.getMinutes() == 37) || (now.getHours() == 23 && now.getMinutes() == 59)){
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