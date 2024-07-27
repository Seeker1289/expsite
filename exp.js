const dict = ['abacus', 'abandon', 'abbreviate', 'abbreviation', 'abdomen', 'abduct', 'abide', 'ability', 'able', 'abnormal', 'abolish', 'abort', 'about', 'above', 'abroad', 'abrupt', 'abs', 'abscissa', 'absent', 'absentee', 'absolute', 'absorb', 'absorbent', 'abstain', 'abstract', 'absurd', 'abundant', 'abuse', 'abyss', 'acacia', 'academia', 'academy', 'accelerate', 'accelerator', 'accent', 'accept', 'access', 'accession', 'accessory', 'accident', 'accommodate', 'accompany', 'accomplish', 'accomplishment', 'accord', 'account', 'accountable', 'accountant', 'accredit', 'accumulate', 'accuracy', 'accurate', 'accuse', 'accustom', 'ace', 'ache', 'achieve', 'achievement', 'achiever', 'acid', 'acknowledge', 'acknowledgement', 'acne', 'acorn', 'acoustic', 'acoustics', 'acquaint', 'acquaintance', 'acquire', 'acre', 'acrobat', 'acrobatics', 'acronym', 'across', 'acrylic', 'act', 'action', 'activate', 'active', 'activist', 'activity', 'actor', 'actress', 'actual', 'actualize', 'actuary', 'actuate', 'actuator', 'acupressure', 'acupuncture', 'acute', 'adagio', 'adamant', 'adapt', 'adaptable', 'adaptation', 'add', 'addict', 'addiction', 'addition', 'additional', 'additive', 'address', 'adequate', 'adhere', 'adhesive', 'adieu', 'adjacent', 'adjective', 'adjoin', 'adjust', 'adjustable', 'adjustment', 'administer', 'administrate', 'administration', 'admire', 'admission', 'admit', 'adolescent', 'adopt', 'adorable', 'adore', 'adorn', 'adsorb', 'adsorbent', 'adult', 'adulthood', 'advance', 'advancement', 'advantage', 'adventure', 'adverb', 'adversary', 'adversity', 'advertise', 'advertisement', 'advice', 'advise', 'advocate', 'aerodynamic', 'aeronautic', 'aeroplane', 'aerosol', 'aesthete', 'aesthetic', 'affair', 'affect', 'affection', 'affectionate', 'affiliate', 'affinity', 'afflict', 'afford', 'affordable', 'afforest', 'afforestation', 'afloat', 'afraid', 'after', 'aftermath', 'afternoon', 'afterward', 'again', 'against', 'age', 'agency', 'agenda', 'agent', 'aggregate', 'aggression', 'aggressive', 'agile', 'agility', 'agitate', 'agitation', 'ago', 'agony', 'agree', 'agreement', 'agriculture', 'ahead', 'aid', 'aide', 'ail', 'ailment', 'aim', 'air', 'airbag', 'airborne', 'aircraft', 'airport', 'airtight', 'aisle', 'akin', 'alarm', 'albatross', 'albino', 'album', 'albumen', 'albumin', 'alchemy', 'alcohol', 'alcoholic', 'alert', 'algae', 'algebra', 'algorithm', 'alien', 'align', 'alike', 'alive', 'alkali', 'alkaline', 'all', 'allege', 'allergy', 'alley', 'alliance', 'alligator', 'allocate', 'allot', 'allow', 'alloy', 'allure', 'ally', 'almighty', 'almond', 'almost', 'alms', 'alone', 'along', 'alongside', 'aloud', 'alpaca', 'alpha', 'alphabet', 'already', 'alright', 'also', 'alter', 'alteration', 'alternate', 'although', 'altitude', 'alumni', 'always', 'amaze', 'amazement', 'ambassador', 'ambidextrous', 'ambience', 'ambiguity', 'ambition', 'ambulance', 'ambush', 'amend', 'amendment', 'amenity', 'ammeter', 'ammo', 'amnesia', 'amoeba', 'among', 'amorphous', 'amount', 'ampersand', 'amphibian', 'amphitheatre', 'ample', 'amplify', 'amulet', 'amuse', 'amusement', 'anaconda', 'anagram', 'analog', 'analogue', 'analogy', 'analysis', 'analyst', 'analytic', 'analyze', 'anatomy', 'ancestor', 'anchor', 'ancient', 'and', 'anemia', 'anesthesia', 'angel', 'anger', 'angle', 'angry', 'anguish', 'animal', 'animate', 'animation', 'animator', 'ankle', 'anklet', 'annihilate', 'anniversary', 'annotate', 'annotation', 'announce', 'announcement', 'annoy', 'annoyance', 'annual', 'anonymous', 'another', 'answer', 'ant', 'antacid', 'antagonist', 'antarctic', 'antelope', 'antenna', 'anthem', 'anthill', 'antibiotic', 'anticipate', 'antidote', 'antimatter', 'antique', 'antiseptic', 'antler', 'antonym', 'anus', 'anxiety', 'anxious', 'any', 'anything', 'apart', 'apartment', 'apathy', 'ape', 'aperture', 'apex', 'apocalypse', 'apologize', 'apology', 'apparatus', 'apparel', 'apparent', 'appeal', 'appear', 'appearance', 'append', 'appendix', 'appetite', 'applaud', 'applause', 'apple', 'applicant', 'application', 'apply', 'appoint', 'appointment', 'appreciate', 'appreciation', 'apprehend', 'apprehension', 'apprentice', 'approach', 'appropriate', 'approve', 'approximate', 'approximation', 'apricot', 'apron', 'aptitude', 'aquarium', 'aquatic', 'aquifer', 'arachnid', 'arbitrary', 'arboreal'];
let rand = Math.floor(Math.random()*dict.length);
let word = dict[rand];
/*let word = prompt('word');*/
let blanks = document.querySelector("#blnk");
const button = document.querySelector("button");
let gss = document.querySelector("#gss");
let a ='';
let b = [];
let count;
let wrongs = 0;
let check;
let n;
let l = word.length;
let letter;
let comma = 0;
document.querySelector('h3').innerText = `It is a ${l} letter word.`;
for (let i of word) {
    b.push("-");
    a = a + '-';
    blanks.innerText = a;
}
/*button.onclick = () => {blanks.innerText = document.querySelector('input').value;};*/
button.onclick = function() {
    letter = document.querySelector('input').value.toLowerCase();
    if (letter.length == 1) {
        wrongs++;
        a = '';
        count = 0;
        check = 0;
        n = 0;
        for (i=0; i<l; i++) {
            if (word[i] == letter) {
                b[i] = letter;
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
            document.querySelector('h3').innerText = `${letter} is  not there.`;
            if (comma != 0) {
                gss.innerText =  gss.innerText + ',' + letter;
            } else {
                gss.innerText = letter;
                comma++;
            }
        } else {
            document.querySelector('h3').innerText = '';
        }
        if (count == l) {
            document.querySelector('h3').innerText = `You guessed it.`;
        }
        document.querySelector('h4').innerText = `${8-wrongs} chances remaining`;
        blanks.innerText = a;
        if (wrongs > 7) {
            document.querySelector('h3').innerText = `You failed to guess it.`;
            blanks.innerText = word;
        }
    } else {
        document.querySelector('h3').innerText = 'Give a valid input. It should contain one letter.'
    }
}
