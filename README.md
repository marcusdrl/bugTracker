# bugTracker

Este projeto foi criado com o intuito de demonstrar um pouco do que eu aprendi com QT e C++, exclusivamente como portfólio.

<h1>COMO USAR</h1>
Primeiramente para se conectar a aplicação você terá de fazer o download dela (<a href="https://github.com/marcusdrl/bugTracker/releases/download/1.1/MVbugTracker.exe">Installer</a> ou <a href="https://github.com/marcusdrl/bugTracker/releases/download/1.1/MVbugTracker.rar">.RAR</a>). Após realizar o download e a instalação, você irá abrir a aplicação e 
irá se registrar com uma conta, preenchendo corretamente os campos porque sua conta terá que ser ativada, então cuidado nesta parte.
<br>
PS: O windows pode barra a execução do installer devido a ausência do certicado  de publisher, se optarem pelo installer, por favor sigam este tutorial:https://social.technet.microsoft.com/wiki/contents/articles/26352.o-windows-protegeu-o-computador-o-windows-smartscreen-impediu-a-inicializacao-de-um-aplicativo-nao-reconhecido-se-voce-executar-esse-aplicativo-o-pc-podera-ficar-vulneravel.aspx
<br>

![cadastrar](https://user-images.githubusercontent.com/71731401/133265459-5c05a971-c51e-4c8b-ba27-0c72b679c4f9.png)

### IMPORTANTE
Ao registrar sua conta você deverá escolher um cargo de acesso(este pode ser modificado dentro do próprio app, com algumas condições). Cargos são de extrema importância pois eles te limitam dentro do que você pode fazer ou acessar dentro da aplicação, vou tentar resumir a diferença entre os cargos abaixo:
* <p align="justify"><strong>ADMINISTRADOR (ADMIN):</strong> Você deverá escolher este cargo quando quiser trabalhar em aplicações pessoais, ou caso você realmente tenha um empresa na qual você possui funcionários os quais necessitam de alguém para gerenciá-los. Como administrador você poderá aceitar novos membros na sua equipe pela aba de aplicações, delegar funções(cargo 'Manager'), criar projetos, tickets e etc;</p>
* <p align="justify"><strong>DESENVOLVEDOR SR./JR.:</strong> Os cargos de desenvolvedores não possuem nada de especial, eles somente podem visualizar os projetos em que se encontram, visualizam tickets e podem editar status de tickets(caso já tenha sido resolvido);</p>
* <p align="justify"><strong>MANAGER:</strong> Primeiramente manager não é um cargo de cadastro, é uma função que deve ser delegada por alguém(ADMINISTRADOR) que já possua uma empresa, inicialmente a pessoa que for receber o cargo de MANAGER deverá se cadastrar como DESENVOLVEDOR e posteriormente receber sua "promoção".</p>

Com a sua conta criada você agora irá ativar ela pelo link que você recebeu em seu e-mail para que esteja apto a realizar o login.
Após realizar o login esta será a primeira imagem que você terá de dentro da aplicação:
![login_adm](https://user-images.githubusercontent.com/71731401/133307700-fcb369ab-a6e3-4979-8859-31472dae0234.png)
**A imagem acima representa o primeiro login de um usuário que escolheu se registrar como administrador, o primeiro passo dele será cadastrar uma nova empresa.**
<br>
<br>
<br>
<br>
![login_desenvolvedor](https://user-images.githubusercontent.com/71731401/133308483-9ed7bc11-128a-43c4-b1b7-03e3bd4e58fe.png)
**A imagem acima representa o primeiro login de um usuário que escolheu se registrar como desenvolvedor, o primeiro passo dele é procurar por uma empresa, porém, caso queira ele pode criar a sua própria o que irá alterar seu cargo de cadastro para "ADMIN"**
<br>
<br>
<br>
<br>

Agora temos uma noção do início de cada cadastro, e iremos visualizar a tela de um usuário cadastrado e ja "trabalhando" em algum projeto:
![menu_esquerda](https://user-images.githubusercontent.com/71731401/133266115-030a4d38-f653-41db-8326-ad248eb72280.png)

<p align='justify'>Na esquerda é possível visualizar o menu com todas as opções de usuário, e a direita fica a nossa página inicial (home) do projeto, que sempre irá representar para
o usuário que está acessando os últimos tickets (se for ADMIN será os ultimos tickets da empresa tickets pessoais inclusos, se for MANAGER será os ultimos tickets dos projetos que gerencia juntamente com os tickets pessoais, se for DESENVOLVEDOR somente os tickets pessoais destinados a você irão aparecer) juntamente do resumo do status de prioridade dos tickets(alta, média, baixa). O menu irá variar de acordo com o nível de acesso do usuário, por exemplo a imagem acima representa o menu de um ADMINISTRADOR, ou seja o usuário que na hora do cadastro resolveu criar sua própria empresa para gerenciar seu projetos, sejam eles pessoais ou não.</p>

<strong>As opções de menu são auto-explicativas, e para evitar que este fique muito extenso e cansativo de ler, irei partir para a explicação das funções mais importantes em minha visão:</strong>
1. **ABRIR TICKETS:** É a função principal da aplicação, aqui é possivel controlar bugs ou possiveis modificações que forem necessárias e designar para alguém em específico que tome conta do problema. A aba para escolher desenvolvedores irá depender da aba projetos, quando for feita a escolha do projeto todos os desenvolvedores que estiverem registrados nele irão aparecer, também é possível adicionar um arquivo de anexo(até 16mb) este sendo opcional.

![abrir_ticket](https://user-images.githubusercontent.com/71731401/133286039-8af286d3-cf2e-4c42-bf66-8cb797580dda.png)

2. **ABA DE TICKETS:** Todas os botões de exibição de tickets('Página Inicial', 'Meus Tickets', 'Tickets da Equipe', 'Meus Tickets Solucionados', 'Tickets Solucionados(Equipe)') possuem função de double-click, esta por sua vez irá abrir uma aba detalhada do ticket que você selecionou(com os dados de abertura, status, prioridade e etc):

![double_click_ticket](https://user-images.githubusercontent.com/71731401/133273174-ea53ad40-db54-4b55-b8be-e6c6892b495e.png)

![detalhes_tickets](https://user-images.githubusercontent.com/71731401/133284846-5a13b360-32e4-490e-bb31-c97bd50ee4e0.png)
**Acima é possivel visualizar os detalhes do ticket como nº do ticket, quem abriu ele, titulo, status, comentários, histórico de modificações e etc...**

  2.1 **COMENTÁRIOS**: É possível adicionar um novo comentário que será adicionado a aba detalhada do ticket, este pode ser usado para "comunicação" caso seja necessária, ou então para registrar o progresso da solução do problema.
  ![comentarios](https://user-images.githubusercontent.com/71731401/133298988-74dbcfc4-718d-4a2e-b21c-48a4f43ac9ae.png)
  **Ao clicar em "enviar" o comentário é adicionado a tabela juntamente com o nome de quem fez o comentário e a hora(veja abaixo).**
  ![comentario_enviado](https://user-images.githubusercontent.com/71731401/133303773-6881d1b3-fcab-46c8-9e31-0d4b89fcf3f2.png)
  
  2.2 **EDITAR TICKET**: É possível modificar algumas informações do ticket dependendo do seu nível de acesso, caso seja administrador você poderá modificar a prioridade, status e o responsável pelo ticket(managers também fazem a mesma coisa), caso desenvolvedor, somente será possível modificar o status para informar se o problema já foi corrigido ou não.
  ![editar_ticket](https://user-images.githubusercontent.com/71731401/133304943-a698f9ba-02d4-4681-beb8-a8a20f9adb5d.png)
  **Ao enviar as modificações elas ficarão registradas na aba de modificações do ticket, apresentadas pela variável que foi modificada e seus respectivos valores, antigo e atual(veja abaixo).**
  ![ticket_modificado](https://user-images.githubusercontent.com/71731401/133305228-c27827bd-fd4a-4b6f-a0aa-db4b54dcf99d.png)
  2.3 **DOWNLOAD DE ANEXOS:** Quando o ticket possuir alguma arquivo anexado, o botao "Download Anexo" ficará visível. Para salvar o arquivo basta clicar no botão e selecionar o caminho que desejar.
  ![download](https://user-images.githubusercontent.com/71731401/133306694-93cb72ff-a1a5-4939-83ca-ff2419aabaaf.png)

<br>
<strong>Acho que isso é tudo que eu tenho a acrescentar, é um programa simples, como mencionei o menu é auto-explicativo vocês não devem ter dificuldade em entender, o resumo acima foi somente para dar uma ideia do que o app possui antes de você realmente utilizá-lo, espero que vocês gostem.</strong>
<br>
<br>
<strong>PS: Caso tenham encontrado algum bug ou queiram entrar em contato -> marcusviniciusdrl@gmail.com.</strong>
  
  
