let word = prompt('word');
let blanks = document.querySelector("h1");
const button = document.querySelector("button");
let a ='';
let b = [];
let count;
let wrongs = 0;
let check;
let n;
let l = word.length;
for (let i of word) {
    b.push("-");
    a = a + '-';
    blanks.innerText = a;
}
/*button.onclick = () => {blanks.innerText = document.querySelector('input').value;};*/
button.onclick = function() {
    wrongs++;
    a = '';
    count = 0;
    check = 0;
    n = 0;
    for (i=0; i<l; i++) {
        if (word[i] == document.querySelector('input').value) {
            b[i] = document.querySelector('input').value;
            a = a + b[i];
            wrongs--;
            if (check != 0) {
                wrongs++;
            }
            check++;
        } else {
            a = a + b[i];
            n++;
        }
    }
    for (i=0; i<l; i++) {
        if (b[i] != '-') {
            count++;
        }
    }
    if (n == l) {
        document.querySelector('h3').innerText = `${document.querySelector('input').value} is  not there.`;
    } else {
        document.querySelector('h3').innerText = '';
    }
    if (count == l) {
        document.querySelector('h3').innerText = `You guessed it.
${wrongs} wrongs.`;
    }
    document.querySelector('h4').innerText = `${8-wrongs} chances remaining`;
    blanks.innerText = a;
    if (wrongs > 7) {
        document.querySelector('h3').innerText = `You failed to guess it.`;
        blanks.innerText = word;
    }
}